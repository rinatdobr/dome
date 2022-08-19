#include "parser.h"

#include <spdlog/spdlog.h>

#include "cputemperature.h"
#include "statistic.h"
#include "roomhumidity.h"
#include "roomtemperature.h"

namespace command {

std::vector<std::string> ParseArgs(const std::string &args)
{
    if (args.size() == 0) {
        return {};
    }

    std::vector<std::string> result;

    std::size_t start = 0;
    int i = 0;
    while (start != std::string::npos) {
        std::size_t end = args.find(' ', start);
        if (end != std::string::npos) {
            result.push_back(std::string(args, start, end - start));
            spdlog::debug("arg[{}]={}", i++, result.back());
        }
        else {
            result.push_back(std::string(args, start));
            spdlog::debug("arg[{}]={}", i++, result.back());
            break;
        }
        start = end + 1;
    }

    return result;
}

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

    return nullptr;
}

}
