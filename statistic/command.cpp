#include "command.h"

#include <iostream>

Command::Type StrToType(const std::string &type)
{
    if (type == "db") {
        return Command::Type::Db;
    }
    else if (type == "file") {
        return Command::Type::File;
    }
    else {
        return Command::Type::Invalid;
    }
}

Command::Command(
    std::unique_ptr<command::Command> &&command,
    uint period,
    const std::string &type
)
    : m_command(std::move(command))
    , m_periodSec(period)
    , m_nextTimeFrameSec(0)
    , m_type(StrToType(type))
{

}

const std::unique_ptr<command::Command> &Command::getCommand() const
{
    return m_command;
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

Command::Type Command::getType()
{
    return m_type;
}

std::shared_ptr<Writer> Command::getWriter()
{
    return m_writer;
}

void Command::setWriter(std::shared_ptr<Writer> writer)
{
    m_writer = writer;
}

command::Result Command::execute()
{
    return m_command->execute();
}
