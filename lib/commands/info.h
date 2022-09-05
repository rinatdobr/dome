#ifndef COMMANDS_INFO_H
#define COMMANDS_INFO_H

#include "command.h"
#include "result.h"

namespace command {

const std::string InfoName("info");
const CommandId InfoCommandId("37248ffa-2cd6-11ed-a206-abdf2f98387b");

class Info : public Command
{
public:
    Info(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
