#include "config.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <nlohmann/json.hpp>

#include <commands/parser.h>

#include "writers/dbwriter.h"

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

std::vector<std::unique_ptr<Command>> &&Config::commands()
{
    return std::move(m_commands);
}

void Config::parse(const std::string &configData)
{
    std::set<std::string> outputs;

    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDatabase = jConfig["database"];
    std::cout << "jDatabase " << jDatabase << std::endl;
    std::string dbName =  jDatabase["name"].get<std::string>();
    std::cout << "dbName " << dbName << std::endl;
    std::string dbEnv = std::getenv(jDatabase["env"].get<std::string>().c_str());
    std::cout << "dbEnv " << dbEnv << std::endl;
    std::string fullDbPath = dbEnv + "/" + dbName;
    outputs.insert(fullDbPath);
    auto dbWriter = std::make_shared<DbWriter>(fullDbPath);

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

            switch (command->getType()) {
                case Command::Type::Db:
                    command->setWriter(dbWriter);
                break;
            }

            m_commands.emplace_back(
                std::move(command)
            );
        }
    }
}
