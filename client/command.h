#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

#include "result.h"

using CommandId = std::string;

class Command
{
public:
    Command(const CommandId &commandId, const std::vector<std::string> &args);
    virtual ~Command();

    virtual Result execute() = 0;

protected:
    CommandId m_commandId;
    std::vector<std::string> m_args;
};

#endif
