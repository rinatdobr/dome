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
        auto _this = static_cast<Reciever*>(userData);

        spdlog::debug("got a message {} from {} on {}", static_cast<char*>(mosqMessage->payload), mosqMessage->topic, _this->m_mosq.clientId());

        std::string topic(mosqMessage->topic);
        std::string message(static_cast<char*>(mosqMessage->payload), mosqMessage->payloadlen);
        nlohmann::json jMessage = nlohmann::json::parse(message);
        if (jMessage["type"] == "ping") {
            spdlog::debug("got a ping message from {}", mosqMessage->topic);
        }

        switch (_this->m_type) {
            case Type::Provider:
                for (const auto &provider : _this->m_providers) {
                    if (provider.id() == topic) {
                        for (auto dataProcessor : _this->m_dataProcessors) {
                            dataProcessor->process(_this->m_mosq, provider, jMessage);
                        }
                    }
                }
            break;
            case Type::Request:
            case Type::Reply:
                for (auto dataProcessor : _this->m_dataProcessors) {
                    dataProcessor->process(_this->m_mosq, _this->m_provider, jMessage);
                }
            break;
        }
    });
}

void Reciever::subscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res;
    switch (m_type) {
        case Type::Provider:
            for (const auto &provider : m_providers) {
                res = mosquitto_subscribe(m_mosq.mosq(), NULL, provider.id().c_str(), 0);
                if (res == MOSQ_ERR_SUCCESS) {
                    spdlog::debug("subscribed on the {}", provider.id());
                }
                else {
                    spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", provider.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
                }
            }
        break;
        case Type::Request:
            res = mosquitto_subscribe(m_mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str(), 0);
            if (res == MOSQ_ERR_SUCCESS) {
                spdlog::debug("subscribed on the {}", GetRequestTopic(m_provider.id()));
            }
            else {
                spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        break;
        case Type::Reply:
            res = mosquitto_subscribe(m_mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str(), 0);
            if (res == MOSQ_ERR_SUCCESS) {
                spdlog::debug("subscribed on the {}", GetRequestTopic(m_provider.id()));
            }
            else {
                spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        break;
    }
}

void Reciever::unsubscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res;
    switch (m_type) {
        case Type::Provider:
            for (const auto &provider : m_providers) {
                res = mosquitto_unsubscribe(m_mosq.mosq(), NULL, provider.id().c_str());
                if (res == MOSQ_ERR_SUCCESS) {
                    spdlog::debug("unsubscribed from the {}", provider.id());
                }
                else {
                    spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", provider.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
                }
            }
        break;
        case Type::Request:
            res = mosquitto_unsubscribe(m_mosq.mosq(), NULL, GetRequestTopic(m_provider.id()).c_str());
            if (res == MOSQ_ERR_SUCCESS) {
                spdlog::debug("unsubscribed from the {}", GetRequestTopic(m_provider.id()));
            }
            else {
                spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        break;
        case Type::Reply:
            res = mosquitto_unsubscribe(m_mosq.mosq(), NULL, GetReplyTopic(m_provider.id()).c_str());
            if (res == MOSQ_ERR_SUCCESS) {
                spdlog::debug("unsubscribed from the {}", GetRequestTopic(m_provider.id()));
            }
            else {
                spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        break;
    }
}

void Reciever::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    subscribe();

    while (m_isWorking) {
        spdlog::debug("recieving on {}...", m_mosq.clientId());

        mosquitto_loop(m_mosq.mosq(), -1, 1);

        if (m_mosq.decrementKeepAlive()) {
            auto message = dome::mosq::Mosquitto::PingMessage();
            spdlog::debug("sending ping message {} from {}", message, m_mosq.clientId());

            int res;
            switch (m_type) {
                case Type::Request:
                    res = mosquitto_publish(m_mosq.mosq(), nullptr, GetRequestTopic(m_provider.id()).c_str(), message.size(), message.c_str(), 0, false);
                    if (res != MOSQ_ERR_SUCCESS) {
                        spdlog::error("mosquitto_publish ping to \"{}\" error[{}]: {}", GetRequestTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
                    }
                break;
                case Type::Reply:
                    res = mosquitto_publish(m_mosq.mosq(), nullptr, GetReplyTopic(m_provider.id()).c_str(), message.size(), message.c_str(), 0, false);
                    if (res != MOSQ_ERR_SUCCESS) {
                        spdlog::error("mosquitto_publish ping to \"{}\" error[{}]: {}", GetReplyTopic(m_provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
                    }
                break;
            }
        }
    }

    unsubscribe();
}

}
}
