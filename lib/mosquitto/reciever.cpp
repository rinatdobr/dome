#include "reciever.h"

#include <spdlog/spdlog.h>

#include "utils/utils.h"

namespace dome {
namespace mosq {

Reciever::Reciever(const std::string &mosqClientId, dome::topic::Processor &processor)
    : m_mosq(mosqClientId, this)
    , m_topicProcessor(processor)
{
    spdlog::trace("{}:{} {} mosqClientId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, mosqClientId);

    if (!m_mosq.isValid()) {
        spdlog::error("Invalid mosquitto");
        return;
    }

    setup();

    I_am_valid();
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
        if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;
        if (jMessage["type"] == "ping") {
            spdlog::debug("got a ping message from {}", mosqMessage->topic);
            return;
        }

        _this->m_topicProcessor.process(_this->m_mosq, topic, jMessage);
    });
}

void Reciever::subscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid reciever");
        return;
    }

    m_topicProcessor.subscribe(m_mosq);
}

void Reciever::unsubscribe()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid reciever");
        return;
    }

    m_topicProcessor.unsubscribe(m_mosq);
}

void Reciever::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid reciever");
        return;
    }

    subscribe();

    while (m_isWorking) {
        spdlog::debug("recieving on {}...", m_mosq.clientId());

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (m_mosq.decrementKeepAlive()) {
            auto message = PingMessage();
            spdlog::debug("sending ping message {} from {}", message, m_mosq.clientId());

            m_topicProcessor.sendPingMessage(m_mosq, message);
        }
    }

    unsubscribe();
}

}
}
