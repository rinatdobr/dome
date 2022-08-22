#include "config.h"

#include <spdlog/spdlog.h>

Config::Config()
    : dome::config::IpCamera(std::getenv("DOME_IP_CAMERAS_CONFIG"))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}
