#include "sender.h"

#include <spdlog/spdlog.h>
#include <sstream>

namespace dome {
namespace mqtt {

Sender::Sender(const dome::config::Config &config, dome::data::Provider &provider)
    : m_mosq(nullptr)
    , m_provider(provider)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error:", mosquitto_strerror(res));
        return;
    }

    m_mosq = mosquitto_new(config.id().data(), true, nullptr);
    if (!m_mosq) {
        spdlog::error("mosquitto_new error");
        return;
    }

    res = mosquitto_connect(m_mosq, "localhost", 1883, 60);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error:", mosquitto_strerror(res));
        return;
    }
}

Sender::~Sender()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_disconnect(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_disconnect error:", mosquitto_strerror(res));
        return;
    }

    mosquitto_destroy(m_mosq);
}

void Sender::start(int period)
{
    spdlog::trace("{}:{} {} period={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, period);

    while (1) {
        if (m_provider.prepareData()) {
            auto a1 = m_provider.getReaderForFloat("aaed98aa-535a-11ed-84c0-4f35a8d3364f");
            auto a2 = m_provider.getReaderForFloat("3968ec9e-535e-11ed-88cf-b73d78fb3403");
            std::ostringstream data;
            data << (*a1)() << " " << (*a2)();
            std::string d = data.str();

            mosquitto_publish(m_mosq, nullptr, "test", d.size(), d.c_str(), 0, false);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(period));
    }
}

// std::string Sender::generateTopicName(const dome::config::Config &config)
// {
//     spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

//     std::ostringstream topicName;
//     // topicName << config.location() << "-" << dome::config::Config::SourceTypeToStr(config.sourceType()) << "-" << config.name();

//     return topicName.str();
// }

}
}
