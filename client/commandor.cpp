#include "commandor.h"

#include <commands/parser.h>

Commandor::Commandor()
{

}

command::Result Commandor::Run(const std::string &commandLine)
{
    auto command = command::Parser::Parse(commandLine);
    if (command) {
        return command->execute();
    }

    return {};
}
