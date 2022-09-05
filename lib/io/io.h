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
enum class Type
{
    Undefined,
    Data,
    Chart
};

    static Type StrToType(const std::string &str);

    virtual void write(const command::Result &result) = 0;
    virtual std::string readLastForSec(uint seconds, Type type);
    virtual std::string readLastForSec(const std::string &name, uint seconds, Type type);
};

}
}

#endif
