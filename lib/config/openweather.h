#ifndef CONFIG_OPEN_WEATHER_H
#define CONFIG_OPEN_WEATHER_H

#include <string>

#include "utils/file.h"

namespace dome {
namespace config {

class OpenWeather : public dome::utils::File
{
public:
    explicit OpenWeather(const std::string &path);
    std::string apiKey() const;
    std::string latitude() const;
    std::string longitude() const;

private:
    bool parse();

    std::string m_apiKey;
    std::string m_latitude;
    std::string m_longitude;
};

}
}

#endif
