#include "commands.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <commands/parser.h>
#include <utils.h>

namespace dome {
namespace config {

Commands::Commands(const std::string &configPath)
    : Config(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parseConfig();
}

std::vector<std::unique_ptr<Command>> &Commands::commands()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_commands;
}

void Commands::parseConfig()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = readConfigFile();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jCommands = jConfig["commands"];
    for(const auto &jCommand : jCommands) {
        if (jCommand.size() != 1) {
            spdlog::error("Invalid command format: \" {} \"", jCommand.dump());
            continue;
        }

        for(const auto &jCommandItem : jCommand.items()) {
            auto props = jCommandItem.value();
            std::string commandName = jCommandItem.key();
            std::string period = props["period"].get<std::string>();
            std::string args = props["args"].get<std::string>();
            std::string outputType = props["output_type"].get<std::string>();
            spdlog::info("Config command: name={} period={} args={} outputType={}", commandName, period, args, outputType);

            auto parsedCommand = command::Parser::Parse(commandName + ' ' + args);
            if (!parsedCommand) {
                continue;
            }
            
            auto command = std::make_unique<Command>(
                std::move(parsedCommand),
                PeriodToSeconds(period),
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
