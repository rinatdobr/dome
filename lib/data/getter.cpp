#include "getter.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace data {

Getter::Getter(dome::mosq::Sender::Trigger &senderTrigger)
    : m_senderTrigger(senderTrigger)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Getter::~Getter()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Getter::process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["name"] == "get") {
        m_senderTrigger.cv.notify_one();
    }
}

}
}