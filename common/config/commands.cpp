#include "commands.h"

#include <iostream>
#include <nlohmann/json.hpp>

#include <commands/parser.h>

namespace {
    
uint ParsePeriod(const std::string &period)
{
    std::cout << "period: " << period << std::endl;

    if (period.rfind("sec") != std::string::npos) {
        // nothing to do
    }

    std::istringstream parser(period);
    unsigned int result;
    parser >> result;

    std::cout << "result: " << result << std::endl;

    return result;
}

}

namespace dome {
namespace config {

Commands::Commands(const std::string &configPath)
    : Config(configPath)
{
    parseConfig();
}

std::vector<std::unique_ptr<Command>> &Commands::commands()
{
    return m_commands;
}

void Commands::parseConfig()
{
    auto configData = readConfigFile();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jCommands = jConfig["commands"];
    for(const auto &jCommand : jCommands) {
        if (jCommand.size() != 1) {
            std::cerr << "Invalid command notation (size)" << std::endl;
            continue;
        }

        for(const auto &jCommandItem : jCommand.items()) {
            std::cout << jCommandItem.key() << " : " << jCommandItem.value() << std::endl;
            auto props = jCommandItem.value();
            std::string commandName = jCommandItem.key();
            std::string period = props["period"].get<std::string>();
            std::string args = props["args"].get<std::string>();
            std::string outputType = props["output_type"].get<std::string>();

            auto parsedCommand = command::Parser::Parse(commandName + ' ' + args);
            if (!parsedCommand) {
                continue;
            }
            
            auto command = std::make_unique<Command>(
                std::move(parsedCommand),
                ParsePeriod(period),
                outputType
            );

            m_commands.emplace_back(
                std::move(command)
            );
        }
    }
}

}
}
