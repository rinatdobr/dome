#include "command.h"

#include <iostream>

namespace dome {
namespace config {

Command::IoType StrToIoType(const std::string &ioType)
{
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

}

Command::IoType Command::ioType()
{
    return m_ioType;
}

uint Command::getPeriodSec() const
{
    return m_periodSec;
}

void Command::setPeriodSec(uint periodSec)
{
    m_periodSec = periodSec;
}

uint Command::nextTimeFrameSec() const
{
    return m_nextTimeFrameSec;
}

void Command::setNextTimeFrameSec(uint nextTimeFrameSec)
{
    if (nextTimeFrameSec == 0) {
        m_nextTimeFrameSec += m_periodSec;
    }
    else {
        m_nextTimeFrameSec = nextTimeFrameSec;
    }
}


std::shared_ptr<dome::io::Io> Command::io()
{
    return m_io;
}

void Command::setIo(std::shared_ptr<dome::io::Io> io)
{
    m_io = io;
}

const std::unique_ptr<command::Command> &Command::command() const
{
    return m_command;
}

}
}
