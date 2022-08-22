#ifndef COMMANDS_IP_CAMERA_H
#define COMMANDS_IP_CAMERA_H

#include "command.h"
#include "result.h"

namespace command {

const std::string IpCameraName("ipcamera");
const CommandId IpCameraCommandId("09cb982c-21d8-11ed-bc8a-b7af49f2da93");

class IpCamera : public Command
{
public:
    IpCamera(const std::vector<std::string> &args);

    std::string name() const override;
    Result execute() override;
};

}

#endif
