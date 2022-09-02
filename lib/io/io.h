#ifndef IO_IO_H
#define IO_IO_H

#include <string>

#include <commands/command.h>
#include <commands/result.h>

namespace dome {
namespace io {

class Io
{
public:
    virtual void write(const command::Result &result) = 0;
    virtual std::string readLastForSec(uint seconds);
    virtual std::string readLastForSec(const std::string &name, uint seconds);
};

}
}

#endif
