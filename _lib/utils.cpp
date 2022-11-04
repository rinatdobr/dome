#include "utils.h"

#include <sstream>
#include <spdlog/spdlog.h>

uint PeriodToSeconds(const std::string &period)
{
    spdlog::trace("{}:{} {} period={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, period);

    std::istringstream parser(period);
    unsigned int result;
    parser >> result;

    if (period.rfind("s") != std::string::npos) {
        // nothing to do
    }
    else if (period.rfind("m") != std::string::npos) {
        result *= 60;
    }
    else if (period.rfind("h") != std::string::npos) {
        result *= 3600;
    }
    else if (period.rfind("d") != std::string::npos) {
        result *= 86400;
    }

    spdlog::debug("{}:{}", __PRETTY_FUNCTION__, result);

    return result;
}

std::vector<std::string> ParseArgs(const std::string &args)
{
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
