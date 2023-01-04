#include "receiver.h"

#include <spdlog/spdlog.h>

#include "message/message.h"
#include "utils/utils.h"

namespace dome {
namespace mosq {

Receiver::Receiver(const std::string &mosqClientId, std::vector<dome::mosq::Topic> topics)
    : m_mosq(mosqClientId + "/receiver", dome::mosq::Topic::GetTopicNames(topics), this)
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

Receiver::~Receiver()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Receiver::setup()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_mosq.setCallback([](struct mosquitto *mosq, void *userData, const struct mosquitto_message *mosqMessage){
        auto _this = static_cast<Receiver*>(userData);

        spdlog::debug("got a message [{}] from \"{}\" on \"{}\"", static_cast<char*>(mosqMessage->payload), mosqMessage->topic, _this->m_mosq.clientId());

        std::string mosqTopic(mosqMessage->topic);
        std::string message(static_cast<char*>(mosqMessage->payload), mosqMessage->payloadlen);
        nlohmann::json jMessage = nlohmann::json::parse(message);
        if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

        if (jMessage["type"] == dome::message::type::Ping) {
            spdlog::debug("got a ping message from \"{}\"", mosqMessage->topic);
            return;
        }
        else {
            spdlog::debug("got a message from \"{}\"", mosqMessage->topic);
        }

        for (auto topic: _this->m_topics) {
            if (mosqTopic == topic.name()) {
                for (auto processor : topic.processors()) {
                    processor->process(_this->m_mosq, topic.endPointConfig(), jMessage);
                }
            }
        }
    });
}

void Receiver::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid receiver");
        return;
    }

    while (m_isWorking) {
        spdlog::debug("recieving on \"{}\"...", m_mosq.clientId());

        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (m_mosq.decrementKeepAlive()) {
            auto message = PingMessage();

            for (const auto &topicName : dome::mosq::Topic::GetTopicNames(m_topics)) {
                spdlog::debug("sending ping message [{}] from \"{}\" to \"{}\"", message, m_mosq.clientId(), topicName);
                m_mosq.publish(topicName, message);
            }
        }
    }
}

}
}
