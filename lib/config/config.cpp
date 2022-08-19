#include "config.h"

#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>

namespace dome {
namespace config {

Config::Config(const std::string &configPath)
    : m_configPath(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);
}

std::string Config::readConfigFile()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::ifstream configFile(m_configPath);
    if (!configFile.is_open()) {
        spdlog::error("Can't open config file {}", m_configPath);
        return {};
    }

    std::ostringstream configFileStream;
    configFileStream << configFile.rdbuf();

    return configFileStream.str();
}

}
}
