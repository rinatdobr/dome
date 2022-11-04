#include "file.h"

#include <sstream>
#include <fstream>
#include <spdlog/spdlog.h>

namespace dome {
namespace config {

File::File(const std::string &path)
    : m_path(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, path);
}

std::string File::read()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::ifstream configFile(m_path);
    if (!configFile.is_open()) {
        spdlog::error("Can't open config file {}", m_path);
        return {};
    }

    std::ostringstream configFileStream;
    configFileStream << configFile.rdbuf();

    return configFileStream.str();
}

}
}
