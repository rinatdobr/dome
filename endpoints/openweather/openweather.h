#ifndef ENDPOINTS_OPEN_WEATHER_H
#define ENDPOINTS_OPEN_WEATHER_H

#include <config/provider.h>
#include <config/openweather.h>
#include <data/provider.h>

#include <memory>

namespace dome {
namespace data {

class OpenWeather : public dome::data::Provider
{
public:
    explicit OpenWeather(const dome::config::OpenWeather &openWeatherConfig, const dome::config::Provider &providerConfig);
    ~OpenWeather();

    virtual bool prepareData() override;
protected:
    virtual nlohmann::json getData() override;
    virtual bool isDataLeft() override;

private:
    const dome::config::OpenWeather &m_openWeatherConfig;
    const dome::config::Provider &m_providerConfig;
    double m_tempreature;
    double m_humidity;
};

}
}

#endif
