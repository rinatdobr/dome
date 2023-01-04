#include "command.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace message {

std::vector<std::string> Command::parseArgs(const std::string &args)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (args.size() == 0) {
        return {};
    }

    std::vector<std::string> result;

    std::size_t start = 0;
    int i = 0;
    while (start != std::string::npos) {
        std::size_t end = args.find(' ', start);
        if (end != std::string::npos) {
            result.push_back(std::string(args, start, end - start));
            spdlog::debug("arg[{}]={}", i++, result.back());
        }
        else {
            result.push_back(std::string(args, start));
            spdlog::debug("arg[{}]={}", i++, result.back());
            break;
        }
        start = end + 1;
    }

    return result;
}

}
}
