#include "provider.h"

#include <spdlog/spdlog.h>

#include "utils/utils.h"

namespace dome {
namespace topic {

Provider::Provider(const std::vector<dome::config::Provider> &providers, std::vector<dome::message::Processor*> &messageProcessors)
    : m_providers(providers)
    , m_messageProcessors(messageProcessors)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Provider::~Provider()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Provider::subscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (const auto &provider : m_providers) {
        int res = mosquitto_subscribe(mosq.mosq(), NULL, provider.id().c_str(), 0);
        if (res == MOSQ_ERR_SUCCESS) {
            spdlog::debug("subscribed on the \"{}\"", provider.id());
        }
        else {
            spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", provider.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        }
    }
}

void Provider::unsubscribe(dome::mosq::Mosquitto &mosq)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (const auto &provider : m_providers) {
        int res = mosquitto_unsubscribe(mosq.mosq(), NULL, provider.id().c_str());
        if (res == MOSQ_ERR_SUCCESS) {
            spdlog::debug("unsubscribed from the \"{}\"", provider.id());
        }
        else {
            spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", provider.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        }
    }
}

void Provider::process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (const auto &provider : m_providers) {
        if (provider.id() == topic) {
            for (auto messageProcessor : m_messageProcessors) {
                messageProcessor->process(mosq, provider, jMessage);
            }
        }
    }
}

void Provider::sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return;
}

}
}
