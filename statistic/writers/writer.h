#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <memory>

#include <commands/command.h>
#include <commands/result.h>

class Writer
{
public:
    virtual void write(const std::unique_ptr<command::Command> &command, const command::Result &result) = 0;

};

#endif
