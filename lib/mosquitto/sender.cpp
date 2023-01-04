#include "sender.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <cerrno>

#include "utils/utils.h"

namespace dome {
namespace mosq {

Sender::Sender(const std::string &mosqClientId, const dome::config::EndPoint &endPointConfig, dome::data::Provider &provider, Trigger &trigger)
    : m_mosq(mosqClientId + "/sender", {}, this)
    , m_endPointConfig(endPointConfig)
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
    bool sendByDemand = !m_endPointConfig.periodSec();
    std::chrono::seconds prepareDataTsSec(0);
    bool isDataPrepared = false;
    while (m_isWorking) {
        if (((!sendByDemand && leftSec == 0) || triggered)) {
            auto now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
            auto diff = now - prepareDataTsSec;
            if (diff > std::chrono::seconds(m_endPointConfig.maxFrequenceSec())) {
                isDataPrepared = m_provider.prepareData();
                prepareDataTsSec = now;
            }
            else {
                spdlog::warn("Data preparation is too often in \"{}\", sending old values...", m_mosq.clientId());
            }

            if (isDataPrepared) {
                bool isFirstGet = true;
                while (isFirstGet || m_provider.isDataLeft()) {
                    nlohmann::json jData = m_provider.getData();
                    std::string data = jData.dump();
                    spdlog::debug("sending message [{}] to \"{}\" from \"{}\"", data, m_endPointConfig.id(), m_mosq.clientId());
                    m_mosq.publish(m_endPointConfig.id(), data);

                    isFirstGet = false;
                }
            }
        }

        if (leftSec == 0) {
            leftSec = m_endPointConfig.periodSec();
        }

        if (triggered) {
            triggered = false;
        }
        
        auto status = m_trigger.cv.wait_for(ul, std::chrono::seconds(1));
        if (status == std::cv_status::timeout) {
            spdlog::trace("cv timeout");
            --leftSec;
        }
        else {
            spdlog::trace("cv no timeout");
            triggered = true;
        }

        if (m_mosq.decrementKeepAlive()) {
            auto message = PingMessage();
            spdlog::debug("sending ping message \"{}\" from \"{}\"", message, m_mosq.clientId());
            m_mosq.publish(m_endPointConfig.id(), message);
        }
    }
}

}
}
