#ifndef ENDPOINTS_DHT22_H
#define ENDPOINTS_DHT22_H

#include <config/endpoint.h>
#include <data/provider.h>

#include <memory>
#include <DHTXXD.h>

namespace dome {
namespace data {

class Dht22 : public dome::data::Provider
{
public:
    explicit Dht22(const dome::config::EndPoint &endPointConfig);
    ~Dht22();

protected:
    virtual bool prepareData() override;
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:
    const dome::config::EndPoint &m_endPointConfig;
    int m_pi;
    DHTXXD_t *m_dht;
    double m_tempreature;
    double m_humidity;
};

}
}

#endif
