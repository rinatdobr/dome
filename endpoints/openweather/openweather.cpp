#include "openweather.h"

#include <spdlog/spdlog.h>
#include <cpr/cpr.h>
#include <utils/utils.h>

namespace dome {
namespace data {

OpenWeather::OpenWeather(const dome::config::OpenWeather &openWeatherConfig, const dome::config::Provider &providerConfig)
    : m_openWeatherConfig(openWeatherConfig)
    , m_providerConfig(providerConfig)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    I_am_valid();
}

OpenWeather::~OpenWeather()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

bool OpenWeather::prepareData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid OpenWeather");
        return false;
    }

    cpr::Response r = cpr::Get(
                        cpr::Url{"https://api.openweathermap.org/data/2.5/weather"},
                        cpr::Parameters{
                            { "lat", m_openWeatherConfig.latitude() },
                            { "lon", m_openWeatherConfig.longitude() },
                            { "appid", m_openWeatherConfig.apiKey() },
                            { "units", "metric" }
                        }
                    );

    spdlog::trace("status_code={}, content-type={}, text={}", r.status_code, r.header["content-type"], r.text);

    if (r.status_code != 200) {
        spdlog::error("Can't get current weather: {}", r.status_code);
        return false;
    }

    nlohmann::json jData = nlohmann::json::parse(r.text);
    if (!CheckJsonMessageForKeys(jData, { "main" }) ||
        !CheckJsonMessageForKeys(jData["main"], { "temp", "humidity" })) {
        spdlog::error("Invalid openweather reply JSON: [{}]", jData.dump());
        return false;
    }

    spdlog::debug("temperature={} humidity={}", jData["main"]["temp"].get<double>(), jData["main"]["humidity"].get<int>());
    m_tempreature = jData["main"]["temp"].get<double>();
    m_humidity = jData["main"]["humidity"].get<int>();

    return true;
}

nlohmann::json OpenWeather::getData()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid OpenWeather");
        return {};
    }

    nlohmann::json jData;
    jData["type"] = "data";
    for (const auto &source : m_providerConfig.sources()) {
        if (source.type == dome::config::Source::Type::Temperature) {
            jData[source.id] = m_tempreature;
        }
        else if (source.type == dome::config::Source::Type::Humidity) {
            jData[source.id] = m_humidity;
        }
    }

    return jData;
}

bool OpenWeather::isDataLeft()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return false;
}

}
}
