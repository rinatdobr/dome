#include "config.h"

#include <spdlog/spdlog.h>

Config::Config()
    : dome::config::Db(std::getenv("DOME_DB_CONFIG"))
    , dome::config::Commands(std::getenv("DOME_COMMANDS_CONFIG"))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (auto &command : commands()) {
        if (command->ioType() == dome::config::Command::IoType::Db) {
            command->setIo(io());
        }
    }
}
