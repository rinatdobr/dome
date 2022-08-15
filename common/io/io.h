#ifndef WRITER_H
#define WRITER_H

#include <fstream>
#include <memory>

#include <commands/command.h>
#include <commands/result.h>

namespace dome {
namespace io {

class Io
{
public:
    virtual void write(const command::Result &result) = 0;
    virtual std::string readLastForSec(const command::Command &command, uint seconds) = 0;
};

}
}

#endif
