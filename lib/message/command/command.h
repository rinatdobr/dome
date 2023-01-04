#ifndef MESSAGE_COMMAND_COMMAND_H
#define MESSAGE_COMMAND_COMMAND_H

#include <string>
#include <vector>

namespace dome {
namespace message {

class Command
{
protected:

    std::vector<std::string> parseArgs(const std::string &args);
};

}
}

#endif
