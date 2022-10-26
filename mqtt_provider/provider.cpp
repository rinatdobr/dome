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

}
}
