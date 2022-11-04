#include "sender.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <sstream>

namespace dome {
namespace mqtt_provider {

Sender::Sender(const dome::config::Provider &config, dome::mqtt_provider::Provider &provider)
    : m_config(config)
    , m_mosq(nullptr)
    , m_provider(provider)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error:", mosquitto_strerror(res));
        return;
    }

    m_mosq = mosquitto_new(config.id().data(), true, nullptr);
    if (!m_mosq) {
        spdlog::error("mosquitto_new error");
        return;
    }

    res = mosquitto_connect(m_mosq, "localhost", 1883, 60);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error:", mosquitto_strerror(res));
        return;
    }
}

Sender::~Sender()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_disconnect(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_disconnect error:", mosquitto_strerror(res));
        return;
    }

    mosquitto_destroy(m_mosq);
}

void Sender::start(int period)
{
    spdlog::trace("{}:{} {} period={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, period);

    while (1) {
        if (m_provider.prepareData()) {
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
                }
                
            }

            std::string data = jData.dump();
            mosquitto_publish(m_mosq, nullptr, m_config.id().c_str(), data.size(), data.c_str(), 0, false);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(m_config.periodSec()));
    }
}

}
}
