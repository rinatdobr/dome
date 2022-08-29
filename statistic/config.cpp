#include "config.h"

#include <spdlog/spdlog.h>

Config::Config()
    : dome::config::Db(std::getenv("DOME_DB_CONFIG"))
    , dome::config::Statistic(std::getenv("DOME_STATISTIC_CONFIG"))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}
