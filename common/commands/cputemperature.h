#ifndef COMMANDS_CPU_TEMPERATURE_H
#define COMMANDS_CPU_TEMPERATURE_H

#include "command.h"
#include "result.h"

namespace command {

const std::string CpuTemperatureName("cpu_temperature");
const CommandId CpuTemperatureCommandId("a37ca992-199b-11ed-a829-eb288ac0e568");

class CpuTemperature : public Command
{
public:
    CpuTemperature(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
