#ifndef COMMANDOR_H
#define COMMANDOR_H

#include <string>
#include <vector>
#include <memory>

#include "command.h"

class Parser
{
public:
    static std::unique_ptr<Command> Parse(const std::string &line);

private:
    Parser();
};

class Commandor
{
public:
    Commandor();

    static Result Run(const std::string &commandLine);
private:
};

#endif
