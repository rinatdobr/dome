#include "dbsaver.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "message/message.h"
#include "utils/utils.h"

namespace dome {
namespace message {

DbSaver::DbSaver(const dome::config::Database &config)
    : m_config(config)
    , m_dbWriter(m_config.path())
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!m_dbWriter.isValid()) {
        return;
    }

    I_am_valid();
}

DbSaver::~DbSaver()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void DbSaver::process(dome::mosq::Mosquitto &, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!endPointConfig.savable()) {
        spdlog::trace("{}:{} {} no need to save...", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    if (!isValid()) {
        spdlog::error("Invalid DB saver");
        return;
    }

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] != dome::message::type::Data) {
        spdlog::trace("{}:{} {} ignore...", __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return;
    }

    if (!m_timestamps.count(endPointConfig.id())) {
        m_timestamps[endPointConfig.id()] = std::chrono::seconds(0);
    }

    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    if (now - m_timestamps[endPointConfig.id()] > std::chrono::seconds(endPointConfig.savePeriodSec())) {
        for (const auto &source : endPointConfig.sources()) {
            switch (source.dataType()) {
                case dome::config::Source::DataType::Undefined:
                break;
                case dome::config::Source::DataType::Float: {
                    if (!CheckJsonMessageForKeys(jMessage, { source.id() })) continue;
                    m_dbWriter.write(source.id(), jMessage[source.id()].get<double>());
                }
                break;
                case dome::config::Source::DataType::Int: {
                    if (!CheckJsonMessageForKeys(jMessage, { source.id() })) continue;
                    m_dbWriter.write(source.id(), jMessage[source.id()].get<int>());
                }
                break;
            }
        }
        m_timestamps[endPointConfig.id()] = now;
    }
}

}
}
