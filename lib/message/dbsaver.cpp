#include "dbsaver.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "utils/utils.h"

namespace dome {
namespace message {

DbSaver::DbSaver(const std::string &path)
    : m_dbWriter(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, path);

    if (!m_dbWriter.isValid()) {
        return;
    }

    I_am_valid();
}

DbSaver::~DbSaver()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void DbSaver::process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid DB saver");
        return;
    }

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] != "data") {
        spdlog::trace("{}:{} {} ignore...", __FILE__, __LINE__, __PRETTY_FUNCTION__);
        return;
    }

    if (!m_timestamps.count(provider.id())) {
        m_timestamps[provider.id()] = std::chrono::seconds(0);
    }

    auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    if (now - m_timestamps[provider.id()] > std::chrono::seconds(provider.savePeriodSec())) {
        for (const auto &source : provider.sources()) {
            switch (source.dataType) {
                case dome::config::Source::DataType::Undefined:
                break;
                case dome::config::Source::DataType::Float: {
                    if (!CheckJsonMessageForKeys(jMessage, { source.id })) continue;
                    m_dbWriter.write(source.id, jMessage[source.id].get<double>());
                }
                break;
                case dome::config::Source::DataType::Int: {
                    if (!CheckJsonMessageForKeys(jMessage, { source.id })) continue;
                    m_dbWriter.write(source.id, jMessage[source.id].get<int>());
                }
                break;
            }
        }
        m_timestamps[provider.id()] = now;
    }
}

}
}
