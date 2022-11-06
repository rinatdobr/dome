#include "provider.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace data {

Provider::Provider()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Provider::~Provider()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Reader<double> *Provider::getReaderForFloat(const std::string &name)
{
    spdlog::trace("{}:{} {} name={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name);

    return nullptr;
}

Reader<std::string> *Provider::getReaderForString(const std::string &name)
{
    spdlog::trace("{}:{} {} name={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name);

    return nullptr;
}

}
}
