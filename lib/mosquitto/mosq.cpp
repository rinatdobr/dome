#include "mosq.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace mosq {

Mosquitto::Mosquitto(const std::string &clientId, std::vector<std::string> topics, void *owner)
    : m_clientId(clientId)
    , m_topics(topics)
    , m_keepAliveSec(KeepAliveSec)
{
    spdlog::trace("{}:{} {} clientId={} owner={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            clientId, owner);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }
    spdlog::info("mosquitto library was inited");

    const char *id = clientId.empty() ? nullptr : clientId.data();
    m_mosq = mosquitto_new(id, true, owner);
    if (!m_mosq) {
        spdlog::error("mosquitto_new \"{}\" error[{}]", id, res);
        return;
    }

    reconnect();

    I_am_valid();
}

Mosquitto::~Mosquitto()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    unsubscribe();

    disconnect();

    mosquitto_destroy(m_mosq);

    mosquitto_lib_cleanup();
}

const std::string &Mosquitto::clientId() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_clientId;
}

int Mosquitto::keepAliveSec()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_keepAliveSec;
}

bool Mosquitto::decrementKeepAlive(int diffSec)
{
    spdlog::trace("{}:{} {} diffSec={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            diffSec);

    m_keepAliveSec -= diffSec;
    if (m_keepAliveSec <= 0) {
        m_keepAliveSec = KeepAliveSec;
        return true;
    }

    return false;
}

void Mosquitto::setCallback(void (*messageCallback)(mosquitto *, void *, const mosquitto_message *))
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    mosquitto_message_callback_set(m_mosq, messageCallback);
}

void Mosquitto::publish(const std::string &topic, const std::string &message)
{
    spdlog::trace("{}:{} {} topic={} message={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            topic, message);
    
    bool isOk = false;
    while (!isOk) {
        int res = mosquitto_publish(m_mosq, nullptr, topic.c_str(), message.size(), message.c_str(), 0, false);
        if (res != MOSQ_ERR_SUCCESS) {
            spdlog::error("mosquitto_publish from \"{}\" to \"{}\" error[{}]: {}", m_clientId, topic, res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            reconnect();
            continue;
        }

        isOk = true;
    }
}

void Mosquitto::reconnect()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
    
    spdlog::warn("{} connecting...", m_clientId);

    I_am_not_valid();

    unsubscribe();
    disconnect();

    bool isConnected = false;
    int attempt = 1;
    while (!isConnected) {
        spdlog::warn("{} connecting... Attempt {}", m_clientId, attempt);
        isConnected = connect() && subscribe();
        if (!isConnected) {
            unsubscribe();
            disconnect();
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
        ++attempt;
    }

    I_am_valid();
}

bool Mosquitto::connect()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_connect(m_mosq, "localhost", 1883, m_keepAliveSec);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return false;
    }
    spdlog::info("connected to the mosquitto server");

    res = mosquitto_loop_start(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_loop_start error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return false;
    }
    spdlog::info("the mosquitto loop was started");
    
    return true;
}

void Mosquitto::disconnect()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_loop_stop(m_mosq, true);
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::info("the mosquitto loop was stopped");
    }
    else {
        spdlog::error("mosquitto_loop_stop error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }

    res = mosquitto_disconnect(m_mosq);
    if (res == MOSQ_ERR_SUCCESS) {
        spdlog::info("disconnected from the mosquitto server");
    }
    else {
        spdlog::error("mosquitto_disconnect error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
    }
}

bool Mosquitto::subscribe()
{
    bool isOK = true;
    for (const auto &topic : m_topics) {
        int res = mosquitto_subscribe(m_mosq, NULL, topic.c_str(), 0);
        if (res == MOSQ_ERR_SUCCESS) {
            spdlog::debug("subscribed on the \"{}\"", topic);
        }
        else {
            spdlog::error("mosquitto_subscribe on \"{}\" error[{}]: {}", topic, res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            isOK = false;
            break;
        }
    }

    return isOK;
}

void Mosquitto::unsubscribe()
{
    for (const auto &topic : m_topics) {
        int res = mosquitto_unsubscribe(m_mosq, NULL, topic.c_str());
        if (res == MOSQ_ERR_SUCCESS) {
            spdlog::debug("unsubscribed from the \"{}\"", topic);
        }
        else {
            spdlog::error("mosquitto_unsubscribe from \"{}\" error[{}]: {}", topic, res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        }
    }
}

}
}
