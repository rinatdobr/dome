#include "command.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace config {

Command::IoType StrToIoType(const std::string &ioType)
{
    spdlog::trace("{}:{} {} ioType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, ioType);

    if (ioType == "db") {
        return Command::IoType::Db;
    }
    else if (ioType == "file") {
        return Command::IoType::File;
    }
    else {
        return Command::IoType::Invalid;
    }
}

Command::Command(
    std::unique_ptr<command::Command> &&command,
    uint period,
    const std::string &ioType
)
    : m_command(std::move(command))
    , m_periodSec(period)
    , m_nextTimeFrameSec(0)
    , m_ioType(StrToIoType(ioType))
{
    spdlog::trace("{}:{} {} period={} ioType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, period, ioType);
}

Command::IoType Command::ioType()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_ioType;
}

uint Command::getPeriodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

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

}
}
