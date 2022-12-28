#include "openweather.h"
#include "utils/utils.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

OpenWeather::OpenWeather(const std::string &path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse openweather config file: {}", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
}

std::string OpenWeather::apiKey() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_apiKey;
}

std::string OpenWeather::latitude() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_latitude;
}

std::string OpenWeather::longitude() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_longitude;
}

bool OpenWeather::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "openweather" })) {
        spdlog::error("Invalid openweather main JSON: [{}]", jConfig.dump());
        return false;
    }

    nlohmann::json jOpenWeather = jConfig["openweather"];
    if (!CheckJsonMessageForKeys(jOpenWeather, { "api_key", "latitude", "longitude" })) {
        spdlog::error("Invalid telegram JSON: [{}]", jOpenWeather.dump());
        return false;
    }

    m_apiKey = jOpenWeather["api_key"].get<std::string>();
    m_latitude = jOpenWeather["latitude"].get<std::string>();
    m_longitude = jOpenWeather["longitude"].get<std::string>();

    spdlog::info("OpenWeather: api_key={} latitude={} longitude={}",
                               m_apiKey, m_latitude, m_longitude);

    return true;
}

}
}
