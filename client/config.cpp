#include "config.h"

#include <spdlog/spdlog.h>

Config::Config()
    : dome::config::Telegram(std::getenv("DOME_TELEGRAM_CONFIG"))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}
