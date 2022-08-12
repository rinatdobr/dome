#include "parser.h"

#include "systeminfo.h"
#include "systemsensors.h"

namespace command {

std::unique_ptr<Command> Parser::Parse(const std::string &line)
{
    std::string command(line, 0, line.find(' '));
    std::string args;

    if (command == command::SystemInfoName) {
        return std::make_unique<command::SystemInfo>(std::vector<std::string>{});
    }
    else if (command == command::SystemSensorsName) {
        return std::make_unique<command::SystemSensors>(std::vector<std::string>{});
    }

    return nullptr;
}

}
