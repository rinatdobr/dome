#include "reciever.h"

#include <spdlog/spdlog.h>

#include <utils.h>

namespace dome {
namespace mosq {

Reciever::Reciever(const std::string &mosqClientId, const std::vector<dome::config::Provider> &providers, std::vector<dome::data::Processor*> &dataProcessors)
    : m_providers(providers)
    , m_provider({})
    , m_mosq(mosqClientId, this)
    , m_dataProcessors(dataProcessors)
    , m_type(Type::Provider)
{
    spdlog::trace("{}:{} {} mosqClientId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, mosqClientId);

    setup();
}

Reciever::Reciever(const std::string &mosqClientId, const dome::config::Provider &provider, std::vector<dome::data::Processor*> &dataProcessors, Type type)
    : m_providers({})
    , m_provider(provider)
    , m_mosq(mosqClientId, this)
    , m_dataProcessors(dataProcessors)
    , m_type(type)
{
    spdlog::trace("{}:{} {} mosqClientId={} type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, mosqClientId, type);

    setup();
}

Reciever::~Reciever()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Reciever::setup()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    mosquitto_message_callback_set(m_mosq.mosq(), [](struct mosquitto *mosq, void *userData, const struct mosquitto_message *mosqMessage){
        spdlog::debug("got a message {} on {}", static_cast<char*>(mosqMessage->payload), mosqMessage->topic);

        auto _this = static_cast<Reciever*>(userData);
        std::string topic(mosqMessage->topic);
        std::string message(static_cast<char*>(mosqMessage->payload), mosqMessage->payloadlen);
        nlohmann::json jMessage = nlohmann::json::parse(message);

        switch (_this->m_type) {
            case Type::Provider:
                for (const auto &provider : _this->m_providers) {
                    if (provider.id() == topic) {
                        for (auto dataProcessor : _this->m_dataProcessors) {
                            dataProcessor->process(provider, jMessage);
                        }
                    }
                }
            break;
            case Type::Command:
            case Type::Reply:
                for (auto dataProcessor : _this->m_dataProcessors) {
                    dataProcessor->process(_this->m_provider, jMessage);
                }
            break;
        }
    });
}

void Reciever::subscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    switch (m_type) {
        case Type::Provider:
            for (const auto &provider : m_providers) {
                spdlog::debug("subscribed on the {}", provider.id());
                mosquitto_subscribe(m_mosq.mosq(), NULL, provider.id().c_str(), 0);
            }
        break;
        case Type::Command:
            spdlog::debug("subscribed on the {}", GetRequestTopic(m_provider.id()));
            mosquitto_subscribe(m_mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str(), 0);
        break;
        case Type::Reply:
            spdlog::debug("subscribed on the {}", GetReplyTopic(m_provider.id()));
            mosquitto_subscribe(m_mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str(), 0);
        break;
    }
}

void Reciever::unsubscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    switch (m_type) {
        case Type::Provider:
            for (const auto &provider : m_providers) {
                spdlog::debug("unsubscribed from the {}", provider.id());
                mosquitto_unsubscribe(m_mosq.mosq(), NULL, provider.id().c_str());
            }
        break;
        case Type::Command:
            spdlog::debug("unsubscribed from the {}", GetRequestTopic(m_provider.id()));
            mosquitto_unsubscribe(m_mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str());
        break;
        case Type::Reply:
            spdlog::debug("unsubscribed from the {}", GetRequestTopic(m_provider.id()));
            mosquitto_unsubscribe(m_mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str());
        break;
    }
}

void Reciever::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    subscribe();

    while (m_isWorking) {
        mosquitto_loop(m_mosq.mosq(), -1, 1);
    }

    unsubscribe();
}

}
}
