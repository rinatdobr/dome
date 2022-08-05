#ifndef SYSTEM_INFO_COMMAND_H
#define SYSTEM_INFO_COMMAND_H

#include "command.h"
#include "result.h"

namespace command {

const CommandId SystemInfoCommandId("7b46590c-147b-11ed-a178-ef497ededf48");

class SystemInfo : public Command
{
public:
    SystemInfo(const std::vector<std::string> &args);

    Result execute() override;
};

}

#endif
