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

std::vector<std::string> Provider::topics()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    std::vector<std::string> out;
    for (const auto &provider : m_providers) {
        out.push_back(provider.id());
    }

    return out;
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
