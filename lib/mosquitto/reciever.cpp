#include "reciever.h"

#include <spdlog/spdlog.h>

#include "utils/utils.h"

namespace dome {
namespace mosq {

Reciever::Reciever(const std::string &mosqClientId, std::vector<std::string> topicNames, std::vector<dome::topic::Topic> topics)
    : m_mosq(mosqClientId, topicNames, this)
    , m_topicNames(topicNames)
    , m_topics(topics)
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

    m_mosq.setCallback([](struct mosquitto *mosq, void *userData, const struct mosquitto_message *mosqMessage){
        auto _this = static_cast<Reciever*>(userData);

        spdlog::debug("got a message [{}] from \"{}\" on \"{}\"", static_cast<char*>(mosqMessage->payload), mosqMessage->topic, _this->m_mosq.clientId());

        std::string mosqTopic(mosqMessage->topic);
        std::string message(static_cast<char*>(mosqMessage->payload), mosqMessage->payloadlen);
        nlohmann::json jMessage = nlohmann::json::parse(message);
        if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

        if (jMessage["type"] == "ping") {
            spdlog::debug("got a ping message from \"{}\"", mosqMessage->topic);
            return;
        }
        else {
            spdlog::debug("got a message from \"{}\"", mosqMessage->topic);
        }

        for (auto topic: _this->m_topics) {
            if (mosqTopic == topic.name()) {
                for (auto processor : topic.processors()) {
                    processor->process(_this->m_mosq, topic.config(), jMessage);
                }
            }
        }
    });
}

void Reciever::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid reciever");
        return;
    }

    while (m_isWorking) {
        spdlog::debug("recieving on \"{}\"...", m_mosq.clientId());

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (m_mosq.decrementKeepAlive()) {
            auto message = PingMessage();

            for (const auto &topicName : m_topicNames) {
                spdlog::debug("sending ping message [{}] from \"{}\" to \"{}\"", message, m_mosq.clientId(), topicName);
                m_mosq.publish(topicName, message);
            }
        }
    }
}

}
}
