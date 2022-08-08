#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

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

Config::Config(const std::string &path)
    : m_path(path)
{
    std::ifstream configFile(path);
    if (!configFile.is_open()) {
        std::cerr << "Can't open config file: " << m_path << std::endl;
        return;
    }

    std::ostringstream configFileStream;
    configFileStream << configFile.rdbuf();
    std::cout << configFileStream.str() << std::endl;

    parse(configFileStream.str());
}

std::vector<Command> Config::commands() const
{
    return m_commands;
}

void Config::parse(const std::string &configData)
{
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    auto jCommands = jConfig["commands"];
    for(const auto &jCommand : jCommands) {
        if (jCommand.size() != 1) {
            std::cerr << "Invalid command notation (size)" << std::endl;
            continue;
        }

        for(const auto &jCommandItem : jCommand.items()) {
            std::cout << jCommandItem.key() << " : " << jCommandItem.value() << "\n";
            Command command(jCommandItem.key());
            for(const auto &jCommandProp : jCommandItem.value().items()) {
                if (jCommandProp.key() == "period") {
                    command.setPeriodSec(ParsePeriod(jCommandProp.value()));
                }
            }

            m_commands.push_back(std::move(command));
        }
    }
}
