#include "commandor.h"

#include <spdlog/spdlog.h>

#include <commands/parser.h>

Commandor::Commandor()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

command::Result Commandor::Run(const std::string &commandLine)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto command = command::Parser::Parse(commandLine);
    if (command) {
        spdlog::info("Executing {}...", command->name());
        return command->execute();
    }

    return {};
}
