#include "dht22.h"

#include <spdlog/spdlog.h>
#include <pigpiod_if2.h>

namespace dome {
namespace data {

Dht22::Dht22(const dome::config::Provider &config)
    : m_config(config)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_pi = pigpio_start(nullptr, nullptr);
    if (m_pi < 0) {
        spdlog::error("Can't start pgpio");
        return;
    }

    m_dht = DHTXXD(m_pi, 17, 0, nullptr);
}

Dht22::~Dht22()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

bool Dht22::prepareData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    DHTXXD_manual_read(m_dht);
    DHTXXD_data_t data = DHTXXD_data(m_dht);
    if (data.status == DHT_GOOD) {
        spdlog::debug("temperature: {}", data.temperature);
        spdlog::debug("humidity: {}", data.humidity);
        m_tempreature = data.temperature;
        m_humidity = data.humidity;
        return true;
    }
    else {
        spdlog::error("Can't get temperature: {}", data.status);
        return false;
    }
}

nlohmann::json Dht22::getData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    nlohmann::json jData;
    jData["type"] = "data";
    for (const auto &source : m_config.sources()) {
        if (source.type == dome::config::Source::Type::Temperature) {
            jData[source.id] = m_tempreature;
        }
        else if (source.type == dome::config::Source::Type::Humidity) {
            jData[source.id] = m_humidity;
        }
    }

    return jData;
}

}
}
