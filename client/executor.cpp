#include "executor.h"

Command::Command(CommandId commandId, const std::vector<std::string> &args)
    : m_commandId(commandId)
    , m_args(args)
{

}

Command::~Command()
{

}

SystemInfoCommand::SystemInfoCommand(CommandId commandId, const std::vector<std::string> &args)
    : Command(commandId, args)
{

}

bool SystemInfoCommand::execute()
{
    return true;
}

std::unique_ptr<Command> Parser::Parse(const std::string &line)
{
    if (line == "system_temperature") {
        return std::make_unique<SystemInfoCommand>(CommandId::SystemTemperature, std::vector<std::string>{});
    }

    return nullptr;
}

Executor::Executor()
{

}

bool Executor::ParseAndExecute(const std::string &line)
{
    auto command = Parser::Parse(line);
    if (command) {
        return command->execute();
    }

    return false;
}
