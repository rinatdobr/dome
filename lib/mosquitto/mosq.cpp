#include "mosq.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace mosq {

Mosquitto::Mosquitto(const std::string clientId, void *owner)
    : m_clientId(clientId)
    , m_keepAliveSec(KeepAliveSec)
{
    spdlog::trace("{}:{} {} clientId={} owner={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, clientId, owner);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }
    spdlog::debug("mosquitto library was inited");

    const char *id = clientId.empty() ? nullptr : clientId.data();
    m_mosq = mosquitto_new(id, true, owner);
    if (!m_mosq) {
        spdlog::error("mosquitto_new \"{}\" error[{}]", id, res);
        return;
    }

    res = mosquitto_connect(m_mosq, "localhost", 1883, m_keepAliveSec);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }

    res = mosquitto_loop_start(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_loop_start error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }

    spdlog::debug("connected to the mosquitto server");
}

Mosquitto::~Mosquitto()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_loop_stop(m_mosq, true);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_loop_stop error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }

    res = mosquitto_disconnect(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_disconnect error[{}]: {}", res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
        return;
    }
    spdlog::debug("disconnected from the mosquitto server");

    mosquitto_destroy(m_mosq);

    mosquitto_lib_cleanup();
}

struct mosquitto *Mosquitto::mosq()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_mosq;
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
    spdlog::trace("{}:{} {} diffSec={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, diffSec);

    m_keepAliveSec -= diffSec;
    if (m_keepAliveSec <= 0) {
        m_keepAliveSec = KeepAliveSec;
        return true;
    }

    return false;
}

std::string Mosquitto::PingMessage()
{
    nlohmann::json jMessage;
    jMessage["type"] = "ping";
    return jMessage.dump();
}


}
}
