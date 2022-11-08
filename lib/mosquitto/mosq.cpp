#include "mosq.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace mosq {

Mosquitto::Mosquitto(const std::string clientId, void *owner)
{
    spdlog::trace("{}:{} {} clientId={} owner={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, clientId, owner);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error:", mosquitto_strerror(res));
        return;
    }
    spdlog::debug("mosquitto library was inited");

    const char *id = clientId.empty() ? nullptr : clientId.data();
    m_mosq = mosquitto_new(id, true, owner);
    if (!m_mosq) {
        spdlog::error("mosquitto_new error");
        return;
    }

    res = mosquitto_connect(m_mosq, "localhost", 1883, 60);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error:", mosquitto_strerror(res));
        return;
    }
    spdlog::debug("connected to the mosquitto server");
}

Mosquitto::~Mosquitto()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_disconnect(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_disconnect error:", mosquitto_strerror(res));
        return;
    }
    spdlog::debug("disconnected from the mosquitto server");

    mosquitto_destroy(m_mosq);
}

struct mosquitto *Mosquitto::mosq()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_mosq;
}

}
}
