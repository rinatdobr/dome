#include "statistic.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

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
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

std::string Statistic::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return StatisticName;
}

Result Statistic::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_args.size() < 2) {
        spdlog::error("command::Statistic: Not enought args");
        return {};
    }

    for (auto &configCommand : Config::getInstance().commands()) {
        if (configCommand->command()->name() == m_args[0]) {
            spdlog::info("Executing {}... Done", name());
            return Result(this, configCommand->io()->readLastForSec(*configCommand->command(), std::stoul(m_args[1])));
        }
    }

    spdlog::error("command::Statistic: Invalid command");
    return {};
}

}
