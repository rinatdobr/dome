#include "statistic.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <iostream>

#include "dbusservice.h"

#include <config/db.h>
#include <config/commands.h>

namespace command {

class Config : public dome::config::Db, public dome::config::Commands
{
public:
    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }

private:
    Config()
        : dome::config::Db(std::getenv("DOME_DB_CONFIG"))
        , dome::config::Commands(std::getenv("DOME_COMMANDS_CONFIG"))
    {
        for (auto &command : commands()) {
            if (command->ioType() == dome::config::Command::IoType::Db) {
                command->setIo(io());
            }
        }
    }

public:
    Config(Config const&)           = delete;
    void operator=(Config const&)   = delete;
};


Statistic::Statistic(const std::vector<std::string> &args)
    : Command(StatisticCommandId, args)
{

}

std::string Statistic::name() const
{
    return StatisticName;
}

Result Statistic::execute()
{
    std::cout << "execute()" << std::endl;

    if (m_args.size() < 2) {
        std::cout << "Not enought args" << std::endl;
        return {};
    }

    for (auto &configCommand : Config::getInstance().commands()) {
        if (configCommand->command()->name() == m_args[0]) {
            return Result(this, configCommand->io()->readLastForSec(*configCommand->command(), std::stoul(m_args[1])));
        }
    }

    return {};
}

}
