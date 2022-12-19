#include "sender.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <cerrno>

#include "utils/utils.h"

namespace dome {
namespace mosq {

Sender::Sender(const std::string &mosqClientId, const dome::config::Provider &config, dome::data::Provider &provider, Trigger &trigger)
    : m_mosq(mosqClientId, this)
    , m_config(config)
    , m_provider(provider)
    , m_trigger(trigger)
{
    spdlog::trace("{}:{} {} mosqClientId={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            mosqClientId);

    if (!m_mosq.isValid()) {
        spdlog::error("Invalid mosquitto");
        return;
    }

    I_am_valid();
}

Sender::~Sender()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Sender::Trigger &Sender::trigger()
{
    return m_trigger;
}

void Sender::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!isValid()) {
        spdlog::error("Invalid sender");
        return;
    }

    std::unique_lock<std::mutex> ul(m_trigger.mutex);
    uint leftSec = 0;
    bool triggered = false;
    bool sendByDemand = !m_config.periodSec();
    std::chrono::seconds prepareDataTsSec(0);
    bool isDataPrepared = false;
    while (m_isWorking) {
        if (((!sendByDemand && leftSec == 0) || triggered)) {
            auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
            auto diff = now - prepareDataTsSec;
            if (diff > std::chrono::seconds(m_config.maxFrequenceSec())) {
                isDataPrepared = m_provider.prepareData();
                prepareDataTsSec = now;
            }
            else {
                spdlog::warn("data preparation is too often in \"{}\", sending old values...", m_mosq.clientId());
            }

            if (isDataPrepared) {
                bool isFirstGet = true;
                while (isFirstGet || m_provider.isDataLeft()) {
                    nlohmann::json jData = m_provider.getData();
                    std::string data = jData.dump();
                    spdlog::debug("sending message [{}] to \"{}\" from \"{}\"", data, m_config.id(), m_mosq.clientId());
                    int res = mosquitto_publish(m_mosq.mosq(), nullptr, m_config.id().c_str(), data.size(), data.c_str(), 0, false);
                    if (res != MOSQ_ERR_SUCCESS) {
                        spdlog::error("mosquitto_publish to \"{}\" error[{}]: {}", m_config.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
                    }

                    isFirstGet = false;
                }
            }
        }

        if (leftSec == 0) {
            leftSec = m_config.periodSec();
        }

        if (triggered) {
            triggered = false;
        }
        
        auto status = m_trigger.cv.wait_for(ul, std::chrono::seconds(1));
        if (status == std::cv_status::timeout) {
            spdlog::trace("{}:{} {} cv timeout", __FILE__, __LINE__, __PRETTY_FUNCTION__);
            --leftSec;
        }
        else {
            spdlog::trace("{}:{} {} cv no timeout", __FILE__, __LINE__, __PRETTY_FUNCTION__);
            triggered = true;
        }

        if (m_mosq.decrementKeepAlive()) {
            auto message = PingMessage();
            spdlog::debug("sending ping message \"{}\" from \"{}\"", message, m_mosq.clientId());
            int res = mosquitto_publish(m_mosq.mosq(), nullptr, m_config.id().c_str(), message.size(), message.c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                spdlog::error("mosquitto_publish ping to \"{}\" error[{}]: {}", m_config.id(), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        }
    }
}

}
}
