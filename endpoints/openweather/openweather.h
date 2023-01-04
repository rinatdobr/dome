#ifndef ENDPOINTS_OPEN_WEATHER_H
#define ENDPOINTS_OPEN_WEATHER_H

#include <config/endpoint.h>
#include <config/openweather.h>
#include <data/provider.h>

#include <memory>

namespace dome {
namespace data {

class OpenWeather : public dome::data::Provider
{
public:
    OpenWeather(const dome::config::EndPoint &endPointConfig, const dome::config::OpenWeather &openWeatherConfig);
    ~OpenWeather();

    virtual bool prepareData() override;
protected:
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:
    const dome::config::EndPoint &m_endPointConfig;
    const dome::config::OpenWeather &m_openWeatherConfig;
    double m_tempreature;
    double m_humidity;
};

}
}

#endif
