#ifndef COMMANDS_SYSTEM_INFO_H
#define COMMANDS_SYSTEM_INFO_H

#include "command.h"
#include "result.h"

namespace command {

const std::string SystemInfoName("system_info");
const CommandId SystemInfoCommandId("7b46590c-147b-11ed-a178-ef497ededf48");

class SystemInfo : public Command
{
public:
    SystemInfo(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
