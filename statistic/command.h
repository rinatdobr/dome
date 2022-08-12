#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <memory>

#include <commands/command.h>

#include "writers/writer.h"

class Command
{
public:

enum class Type
{
    Invalid,
    Db,
    File
};

    Command(
        std::unique_ptr<command::Command> &&command,
        uint period,
        const std::string &type
    );

    const std::unique_ptr<command::Command> &getCommand() const;

    uint getPeriodSec() const;
    void setPeriodSec(uint periodSec);

    uint nextTimeFrameSec() const;
    void setNextTimeFrameSec(uint nextTimeFrameSec = 0);

    Type getType();

    std::shared_ptr<Writer> getWriter();
    void setWriter(std::shared_ptr<Writer> writer);

    command::Result execute();

private:
    std::unique_ptr<command::Command> m_command;
    std::shared_ptr<Writer> m_writer;
    uint m_periodSec;
    uint m_nextTimeFrameSec;
    Type m_type;
};

#endif
