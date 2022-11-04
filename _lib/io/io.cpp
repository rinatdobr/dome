#include "io.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace io {

Io::Type Io::StrToType(const std::string &str)
{
    if (str == "data") {
        return Io::Type::Data;
    }
    else if (str == "chart") {
        return Io::Type::Chart;
    }

    return Io::Type::Undefined;
}

std::string Io::readLastForSec(uint seconds, Type type)
{
    spdlog::trace("{}:{} {} seconds={} type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, seconds, type);

    spdlog::warn("{} not implemented", __PRETTY_FUNCTION__);

    return {};
}

std::string Io::readLastForSec(const std::string &name, uint seconds, Type type)
{
    spdlog::trace("{}:{} {} name={} seconds={} type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, seconds, type);
    
    spdlog::warn("{} not implemented", __PRETTY_FUNCTION__);

    return {};
}

}
}
