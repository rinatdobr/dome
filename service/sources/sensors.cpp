#include "sensors.h"

#include <spdlog/spdlog.h>

#include <pigpiod_if2.h>

Sensors::Sensors()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_pi = pigpio_start(nullptr, nullptr);
    if (m_pi < 0) {
        spdlog::error("Can't start pgpio");
        return;
    }

    m_dht.reset(DHTXXD(m_pi, 17, 0, nullptr));
}

double Sensors::roomTemperature() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    DHTXXD_manual_read(m_dht.get());
    DHTXXD_data_t data = DHTXXD_data(m_dht.get());
    if (data.status == DHT_GOOD) {
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, data.temperature);
        return data.temperature;
    }
    else {
        spdlog::error("Can't get temperature: {}", data.status);
    }

    return -1;
}

double Sensors::roomHumidity() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    DHTXXD_manual_read(m_dht.get());
    DHTXXD_data_t data = DHTXXD_data(m_dht.get());
    if (data.status == DHT_GOOD) {
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, data.humidity);
        return data.humidity;
    }
    else {
        spdlog::error("Can't get humidity: {}", data.status);
    }

    return -1;
}
