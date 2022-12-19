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

void Request::subscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_subscribe(mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str(), 0);
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::debug("subscribed on the \"{}\"", GetRequestTopic(m_provider.id()));
    }
    else {
        spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

void Request::unsubscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_unsubscribe(mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str());
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::debug("unsubscribed from the \"{}\"", GetRequestTopic(m_provider.id()));
    }
    else {
        spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
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

    int res = mosquitto_publish(mosq.mosq(), nullptr, GetRequestTopic(m_provider.id()).c_str(), message.size(), message.c_str(), 0, false);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_publish ping to \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

}
}
