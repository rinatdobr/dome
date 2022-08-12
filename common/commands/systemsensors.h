#ifndef COMMANDS_SYSTEM_SENSORS_H
#define COMMANDS_SYSTEM_SENSORS_H

#include "command.h"
#include "result.h"

namespace command {

const std::string SystemSensorsName("system_sensors");
const CommandId SystemSensorsCommandId("a37ca992-199b-11ed-a829-eb288ac0e568");

class SystemSensors : public Command
{
public:
    SystemSensors(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
