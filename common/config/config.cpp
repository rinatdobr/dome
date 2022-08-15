#include "config.h"

#include <iostream>
#include <sstream>
#include <fstream>

namespace dome {
namespace config {

Config::Config(const std::string &configPath)
    : m_configPath(configPath)
{

}

std::string Config::readConfigFile()
{
    std::ifstream configFile(m_configPath);
    if (!configFile.is_open()) {
        std::cerr << "Can't open config file: " << m_configPath << std::endl;
        return {};
    }

    std::ostringstream configFileStream;
    configFileStream << configFile.rdbuf();

    return configFileStream.str();
}

}
}
