#include "sender.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace mosq {

Sender::Sender(const std::string &mosqClientId, const dome::config::Provider &config, dome::data::Provider &provider, Trigger &trigger)
    : m_mosq(mosqClientId, this)
    , m_config(config)
    , m_provider(provider)
    , m_trigger(trigger)
{
    spdlog::trace("{}:{} {} mosqClientId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, mosqClientId);
}

Sender::~Sender()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Sender::Trigger &Sender::trigger()
{
    return m_trigger;
}

void Sender::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::unique_lock<std::mutex> ul(m_trigger.mutex);
    uint leftSec = 0;
    bool triggered = false;
    while (m_isWorking) {
        if ((leftSec == 0 || triggered) && m_provider.prepareData()) {
            nlohmann::json jData;
            for (const auto &source : m_config.sources()) {
                switch (source.dataType) {
                    case dome::config::Source::DataType::Undefined:
                    break;
                    case dome::config::Source::DataType::Float: {
                        auto dataReader = m_provider.getReaderForFloat(source.name);
                        jData[source.name] = (*dataReader)();
                    }
                    break;
                    case dome::config::Source::DataType::String: {
                        auto dataReader = m_provider.getReaderForString(source.name);
                        jData[source.name] = (*dataReader)();
                    }
                    break;
                }
            }

            std::string data = jData.dump();
            mosquitto_publish(m_mosq.mosq(), nullptr, m_config.id().c_str(), data.size(), data.c_str(), 0, false);
        }

        if (leftSec == 0) {
            leftSec = m_config.periodSec();
        }

        if (triggered) {
            triggered = false;
        }
        
        auto status = m_trigger.cv.wait_for(ul, std::chrono::seconds(1));
        if (status == std::cv_status::timeout) {
            spdlog::trace("{}:{} {} timeout", __FILE__, __LINE__, __PRETTY_FUNCTION__);
            --leftSec;
        }
        else {
            spdlog::trace("{}:{} {} no timeout", __FILE__, __LINE__, __PRETTY_FUNCTION__);
            triggered = true;
        }
    }
}

}
}
