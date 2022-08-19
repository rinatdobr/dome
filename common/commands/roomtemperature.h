#ifndef COMMANDS_ROOM_TEMPERATURE_H
#define COMMANDS_ROOM_TEMPERATURE_H

#include "command.h"
#include "result.h"

namespace command {

const std::string RoomTemperatureName("room_temperature");
const CommandId RoomTemperatureCommandId("16b3d55e-1f80-11ed-8d4c-238d0f0f1e3c");

class RoomTemperature : public Command
{
public:
    RoomTemperature(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
