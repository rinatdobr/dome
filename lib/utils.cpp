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
