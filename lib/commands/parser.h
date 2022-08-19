#ifndef COMMANDS_PARSER_H
#define COMMANDS_PARSER_H

#include <string>
#include <vector>
#include <memory>

#include "command.h"

namespace command {

class Parser
{
public:
    static std::unique_ptr<Command> Parse(const std::string &line);

private:
    Parser();
};

}

#endif
