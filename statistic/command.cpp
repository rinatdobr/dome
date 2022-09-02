#include "command.h"

#include <sstream>

#include <spdlog/spdlog.h>

#include <commands/parser.h>
#include <utils.h>

Command::IoType StrToIoType(const std::string &ioType)
{
    spdlog::trace("{}:{} {} ioType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, ioType);

    if (ioType == "db") {
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, Command::IoType::Db);
        return Command::IoType::Db;
    }
    else if (ioType == "dir") {
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, Command::IoType::Dir);
        return Command::IoType::Dir;
    }
    else {
        spdlog::debug("{}:{}", __PRETTY_FUNCTION__, Command::IoType::Invalid);
        return Command::IoType::Invalid;
    }
}

std::vector<std::unique_ptr<Command>> Command::Create(const std::vector<dome::config::Statistic::Config> &statisticConfig)
{
    std::vector<std::unique_ptr<Command>> result;

    for (const auto &config : statisticConfig) {
        std::ostringstream args;
        for (const auto arg : config.m_args) {
            args << arg << ' ';
        }

        auto parsedCommand = command::Parser::Parse(config.m_name + ' ' + args.str());
        if (!parsedCommand) {
            continue;
        }
        
        auto command = std::make_unique<Command>(
            std::move(parsedCommand),
            PeriodToSeconds(config.m_period),
            config.m_outputType,
            config.m_outputName
        );

        result.emplace_back(
            std::move(command)
        );
    }

    spdlog::debug("Total commands to run: {}", result.size());

    return result;
}

Command::Command(std::unique_ptr<command::Command> &&command,
                 uint period,
                 const std::string &ioType,
                 const std::string &ioName)
    : m_command(std::move(command))
    , m_periodSec(period)
    , m_nextTimeFrameSec(0)
    , m_ioType(StrToIoType(ioType))
    , m_ioName(ioName)
{
    spdlog::trace("{}:{} {} period={} ioType={} ioName={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, period, ioType, ioName);
}

std::string Command::ioName() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, m_ioName);
    return m_ioName;
}

Command::IoType Command::ioType() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, m_ioType);
    return m_ioType;
}

uint Command::getPeriodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, m_periodSec);
    return m_periodSec;
}

void Command::setPeriodSec(uint periodSec)
{
    spdlog::trace("{}:{} {} periodSec={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, periodSec);

    m_periodSec = periodSec;
}

uint Command::nextTimeFrameSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, m_nextTimeFrameSec);
    return m_nextTimeFrameSec;
}

void Command::setNextTimeFrameSec(uint nextTimeFrameSec)
{
    spdlog::trace("{}:{} {} nextTimeFrameSec={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, nextTimeFrameSec);

    if (nextTimeFrameSec == 0) {
        m_nextTimeFrameSec += m_periodSec;
    }
    else {
        m_nextTimeFrameSec = nextTimeFrameSec;
    }
}


std::shared_ptr<dome::io::Io> Command::io()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_io;
}

void Command::setIo(std::shared_ptr<dome::io::Io> io)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_io = io;
}

const std::unique_ptr<command::Command> &Command::command() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_command;
}
