#include "command.h"

#include <iostream>

Command::Command(const CommandId &commandId, const std::vector<std::string> &args)
    : m_commandId(commandId)
    , m_args(args)
{

}

Command::~Command()
{

}
