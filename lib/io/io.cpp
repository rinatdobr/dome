#include "io.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace io {

std::string Io::readLastForSec(uint seconds)
{
    spdlog::trace("{}:{} {} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, seconds);

    spdlog::warn("{} not implemented", __PRETTY_FUNCTION__);

    return {};
}

std::string Io::readLastForSec(const std::string &name, uint seconds)
{
    spdlog::trace("{}:{} {} name={} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, seconds);
    
    spdlog::warn("{} not implemented", __PRETTY_FUNCTION__);

    return {};
}

}
}
