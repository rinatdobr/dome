#ifndef COMMANDS_ROOM_HUMIDITY_H
#define COMMANDS_ROOM_HUMIDITY_H

#include "command.h"
#include "result.h"

namespace command {

const std::string RoomHumidityName("room_humidity");
const CommandId RoomHumidityCommandId("61a17eb6-1f8c-11ed-a349-a3f5c6eee445");

class RoomHumidity : public Command
{
public:
    RoomHumidity(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
