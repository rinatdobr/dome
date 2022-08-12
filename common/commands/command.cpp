#include "command.h"

#include <iostream>

namespace command {

Command::Command(const CommandId &commandId, const std::vector<std::string> &args)
    : m_commandId(commandId)
    , m_args(args)
{

}

Command::~Command()
{

}

}
