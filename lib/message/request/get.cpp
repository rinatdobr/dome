#include "get.h"

#include <spdlog/spdlog.h>
#include "utils/utils.h"
#include "message/message.h"

namespace dome {
namespace message {

Get::Get(dome::mosq::Sender::Trigger &senderTrigger)
    : m_senderTrigger(senderTrigger)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Get::~Get()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Get::process(dome::mosq::Mosquitto &, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type", "request" })) return;

    if (jMessage["type"] == dome::message::type::Request &&
        jMessage["request"] == dome::message::request::Get) {
        m_senderTrigger.cv.notify_one();
    }
}

}
}
