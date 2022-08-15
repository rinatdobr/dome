#include "config.h"

#include <iostream>

Config::Config(const std::string &dbConfigPath, const std::string &commandsConfigPath)
    : dome::config::Db(dbConfigPath)
    , dome::config::Commands(commandsConfigPath)
{
    for (auto &command : commands()) {
        if (command->ioType() == dome::config::Command::IoType::Db) {
            command->setIo(io());
        }
    }
}
