#include "dht22.h"

#include <message/message.h>
#include <spdlog/spdlog.h>
#include <pigpiod_if2.h>

namespace dome {
namespace data {

Dht22::Dht22(const dome::config::EndPoint &endPointConfig)
    : m_endPointConfig(endPointConfig)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_pi = pigpio_start(nullptr, nullptr);
    if (m_pi < 0) {
        spdlog::error("Can't start pgpio");
        return;
    }

    m_dht = DHTXXD(m_pi, 17, 0, nullptr);
    if (m_dht == nullptr) {
        spdlog::error("Can't allocate DHT22 structure");
        return;
    }

    I_am_valid();
}

Dht22::~Dht22()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid DHT22");
        return;
    }

    DHTXXD_cancel(m_dht);
}

bool Dht22::prepareData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid DHT22");
        return false;
    }

    DHTXXD_manual_read(m_dht);
    DHTXXD_data_t data = DHTXXD_data(m_dht);
    if (data.status == DHT_GOOD) {
        spdlog::debug("temperature={} humidity={}", data.temperature, data.humidity);
        m_tempreature = data.temperature;
        m_humidity = data.humidity;
        return true;
    }
    else {
        spdlog::error("Invalid DHT22 data: {}", data.status);
        return false;
    }
}

nlohmann::json Dht22::getData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid DHT22");
        return {};
    }

    nlohmann::json jData;
    jData["type"] = dome::message::type::Data;
    for (const auto &source : m_endPointConfig.sources()) {
        if (source.type() == dome::config::Source::Type::Temperature) {
            jData[source.id()] = m_tempreature;
        }
        else if (source.type() == dome::config::Source::Type::Humidity) {
            jData[source.id()] = m_humidity;
        }
    }

    return jData;
}

bool Dht22::isDataLeft()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return false;
}

}
}
