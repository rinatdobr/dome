#include "request.h"

#include <spdlog/spdlog.h>

#include "utils/utils.h"

namespace dome {
namespace topic {

Request::Request(const dome::config::Provider &provider, std::vector<dome::message::Processor*> &messageProcessors)
    : m_provider(provider)
    , m_messageProcessors(messageProcessors)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Request::~Request()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

std::vector<std::string> Request::topics()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return { GetRequestTopic(m_provider.id()) };
}

void Request::process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (auto messageProcessor : m_messageProcessors) {
        messageProcessor->process(mosq, m_provider, jMessage);
    }
}

void Request::sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    mosq.publish(GetRequestTopic(m_provider.id()), message);
}

}
}
