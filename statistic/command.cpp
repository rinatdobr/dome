#include "command.h"

#include <iostream>

Command::Command(const std::string &command)
    : m_command(command)
    , m_periodSec(0)
    , m_nextTimeFrameSec(0)
{

}

const std::string &Command::getCommand() const
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

void Command::execute()
{

}
