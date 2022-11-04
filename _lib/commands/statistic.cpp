#include "statistic.h"

#include <QtDBus/QtDBus>
#include <sstream>
#include <spdlog/spdlog.h>

#include "dbusservice.h"

#include <io/db.h>
#include <config/db.h>
#include <config/statistic.h>
#include <utils.h>

namespace command {

class Config : public dome::config::Db, public dome::config::Statistic
{
public:
    static Config& getInstance()
    {
        static Config instance;
        return instance;
    }

    std::map<std::string, std::shared_ptr<dome::io::Db>> m_dbs;
private:
    Config()
        : dome::config::Db(std::getenv("DOME_DB_CONFIG"))
        , dome::config::Statistic(std::getenv("DOME_STATISTIC_CONFIG"))
        , m_dbs(dome::io::Db::Create(dbConfig()))
    {
    }

public:
    Config(Config const&)           = delete;
    void operator=(Config const&)   = delete;
};

Statistic::Statistic(const std::vector<std::string> &args)
    : Command(StatisticCommandId, args)
{
    spdlog::trace("{}:{} {} args.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, args.size());
}

std::string Statistic::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return StatisticName;
}

Result Statistic::execute()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    // 0 - command name
    // 1 - period
    // 2 - type
    if (m_args.size() < 3) {
        spdlog::error("command::Statistic: Not enought args");
        return Result(this, "Not enought args", Result::Status::Fail);
    }

    dome::io::Io::Type ioType = dome::io::Io::StrToType(m_args[2]);

    for (auto &configCommand : Config::getInstance().statisticConfig()) {
        if (configCommand.m_name == m_args[0]) {
            spdlog::info("Executing {}... Done", name());
            switch (ioType) {
                case dome::io::Io::Type::Data:
                    return Result(this, Config::getInstance().m_dbs[configCommand.m_outputName]->readLastForSec(configCommand.m_name, PeriodToSeconds(m_args[1]), ioType), Result::Status::Success);
                break;
                case dome::io::Io::Type::Chart:
                    return Result(this, Config::getInstance().m_dbs[configCommand.m_outputName]->readLastForSec(configCommand.m_name, PeriodToSeconds(m_args[1]), ioType), Result::FileType::Photo, Result::Status::Success);
                break;
            }
        }
    }

    spdlog::error("command::Statistic: Invalid command");
    return Result(this, "Invalid command", Result::Status::Fail);
}

}
