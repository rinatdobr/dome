#ifndef COMMANDOR_H
#define COMMANDOR_H

#include <string>
#include <vector>
#include <memory>

#include <commands/command.h>

class Commandor
{
public:
    Commandor();

    static command::Result Run(const std::string &commandLine);
private:
};

#endif
