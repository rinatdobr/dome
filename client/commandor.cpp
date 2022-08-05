#include "commandor.h"

#include "commands/systeminfo.h"

std::unique_ptr<Command> Parser::Parse(const std::string &line)
{
    if (line == "system_info") {
        return std::make_unique<command::SystemInfo>(std::vector<std::string>{});
    }

    return nullptr;
}

Commandor::Commandor()
{

}

Result Commandor::Run(const std::string &commandLine)
{
    auto command = Parser::Parse(commandLine);
    if (command) {
        return command->execute();
    }

    return {};
}
