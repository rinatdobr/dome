#include "dht22.h"

#include <spdlog/spdlog.h>
#include <pigpiod_if2.h>

namespace dome {
namespace data {

Dht22::Dht22()
    : m_tempreatureReader(this)
    , m_humidityReader(this)
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
        m_tempreatureReader.value = data.temperature;
        m_humidityReader.value = data.humidity;
        return true;
    }
    else {
        spdlog::error("Can't get temperature: {}", data.status);
        return false;
    }
}

Reader<double> *Dht22::getReaderForFloat(const std::string &name)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (name == "aaed98aa-535a-11ed-84c0-4f35a8d3364f") {
        return &m_tempreatureReader;
    }
    else if (name == "3968ec9e-535e-11ed-88cf-b73d78fb3403") {
        return &m_humidityReader;
    }

    return nullptr;
}

double Dht22::TemperatureReader::operator()()
{
    return value;
}

double Dht22::HumidityReader::operator()()
{
    return value;
}

}
}
