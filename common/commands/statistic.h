#ifndef COMMANDS_STATISTIC_H
#define COMMANDS_STATISTIC_H

#include "command.h"
#include "result.h"

namespace command {

const std::string StatisticName("statistic");
const CommandId StatisticCommandId("ccf7ad1c-1c61-11ed-acfc-2f1cb759c21d");

class Statistic : public Command
{
public:
    Statistic(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
