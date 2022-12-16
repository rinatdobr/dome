#include "reply.h"

#include <spdlog/spdlog.h>

#include "utils/utils.h"

namespace dome {
namespace topic {

Reply::Reply(const dome::config::Provider &provider, std::vector<dome::message::Processor*> &messageProcessors)
    : m_provider(provider)
    , m_messageProcessors(messageProcessors)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Reply::~Reply()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Reply::subscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_subscribe(mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str(), 0);
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::debug("subscribed on the {}", GetReplyTopic(m_provider.id()));
    }
    else {
        spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

void Reply::unsubscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_unsubscribe(mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str());
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::debug("unsubscribed from the {}", GetReplyTopic(m_provider.id()));
    }
    else {
        spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

void Reply::process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (auto dataProcessor : m_messageProcessors) {
        dataProcessor->process(mosq, m_provider, jMessage);
    }
}

void Reply::sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_publish(mosq.mosq(), nullptr, GetReplyTopic(m_provider.id()).c_str(), message.size(), message.c_str(), 0, false);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_publish ping to \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

}
}
