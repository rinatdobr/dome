#include "ping.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace data {

Ping::Ping(dome::mosq::Sender::Trigger &senderTrigger)
    : m_senderTrigger(senderTrigger)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Ping::~Ping()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Ping::process(const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["name"] == "ping") {
        m_senderTrigger.cv.notify_one();
    }
}

}
}
