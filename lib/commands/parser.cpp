#include "parser.h"

#include <spdlog/spdlog.h>

#include "cputemperature.h"
#include "statistic.h"
#include "roomhumidity.h"
#include "roomtemperature.h"
#include "ipcamera.h"

#include <utils.h>

namespace command {

std::unique_ptr<Command> Parser::Parse(const std::string &line)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::size_t delimetr = line.find(' ');
    std::string command(line, 0, delimetr);

    spdlog::debug("command={}", command);
    if (command == command::CpuTemperatureName) {
        return std::make_unique<command::CpuTemperature>(ParseArgs(std::string(line, delimetr + 1)));
    }
    else if (command == command::StatisticName) {
        return std::make_unique<command::Statistic>(ParseArgs(std::string(line, delimetr + 1)));
    }
    else if (command == command::RoomTemperatureName) {
        return std::make_unique<command::RoomTemperature>(ParseArgs(std::string(line, delimetr + 1)));
    }
    else if (command == command::RoomHumidityName) {
        return std::make_unique<command::RoomHumidity>(ParseArgs(std::string(line, delimetr + 1)));
    }
    else if (command == command::IpCameraName) {
        return std::make_unique<command::IpCamera>(ParseArgs(std::string(line, delimetr + 1)));
    }

    return nullptr;
}

}
