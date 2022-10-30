#ifndef PROVIDERS_DHT22_H
#define PROVIDERS_DHT22_H

#include "core/provider.h"

#include <memory>
#include <DHTXXD.h>

namespace dome {
namespace data {

class Dht22 : public dome::core::Provider
{
public:
class TemperatureReader : public dome::core::Reader<double>
{
public:
    explicit TemperatureReader(Dht22 *provider)
        : Reader<double>(provider)
    {}

    double operator()() override;

    friend class Dht22;
};

class HumidityReader : public dome::core::Reader<double>
{
public:
    explicit HumidityReader(Dht22 *provider)
        : Reader<double>(provider)
    {}

    double operator()() override;

    friend class Dht22;
};

    explicit Dht22();
    ~Dht22();

    virtual dome::core::Reader<double> *getReaderForFloat(const std::string &name) override;

protected:
    virtual bool prepareData() override;

private:
    TemperatureReader m_tempreatureReader;
    HumidityReader m_humidityReader;
    int m_pi;
    DHTXXD_t *m_dht;
};

}
}

#endif
