#ifndef COMMANDS_COMMAND_H
#define COMMANDS_COMMAND_H

#include <string>
#include <vector>

#include "result.h"

namespace command {

using CommandId = std::string;

const std::string UndefinedName("undefined");

class Command
{
public:
    Command(const CommandId &commandId, const std::vector<std::string> &args);
    virtual ~Command();

    virtual std::string name() const = 0;
    virtual Result execute() = 0;

protected:
    CommandId m_commandId;
    std::vector<std::string> m_args;
};

}

#endif
