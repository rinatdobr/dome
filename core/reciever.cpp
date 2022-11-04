#include "reciever.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <sstream>

namespace dome {
namespace core {

Reciever::Reciever(const dome::config::Core &config, dome::db::Writer &dbWriter)
    : m_config(config)
    , m_mosq(nullptr)
    , m_dbWriter(dbWriter)
    , m_isWorking(false)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_lib_init();
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_lib_init error:", mosquitto_strerror(res));
        return;
    }

    m_mosq = mosquitto_new("core", true, this);
    if (!m_mosq) {
        spdlog::error("mosquitto_new error");
        return;
    }

    res = mosquitto_connect(m_mosq, "localhost", 1883, 60);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_connect error:", mosquitto_strerror(res));
        return;
    }

    mosquitto_message_callback_set(m_mosq, [](struct mosquitto *mosq, void *userData, const struct mosquitto_message *mosqMessage){
        spdlog::info("Got a message!");

        auto _this = static_cast<Reciever*>(userData);
        std::string topic(mosqMessage->topic);
        std::string message(static_cast<char*>(mosqMessage->payload), mosqMessage->payloadlen);
        nlohmann::json jMessage = nlohmann::json::parse(message);

        for (const auto &provider : _this->m_config.providers()) {
            if (provider.id() == topic) {
                for (const auto &source : provider.sources()) {
                    switch (source.dataType) {
                        case dome::config::Source::DataType::Undefined:
                        break;
                        case dome::config::Source::DataType::Float: {
                            _this->m_dbWriter.write(source.name, jMessage[source.name].get<double>());
                        }
                        break;
                    }
                    
                }
            }
        }
    });
}

Reciever::~Reciever()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int res = mosquitto_disconnect(m_mosq);
    if (res != MOSQ_ERR_SUCCESS) {
        spdlog::error("mosquitto_disconnect error:", mosquitto_strerror(res));
        return;
    }

    mosquitto_destroy(m_mosq);
}

void Reciever::start()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_isWorking = true;

    m_worker = std::thread(&Reciever::backgroundWork, this);
}
void Reciever::stop()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_isWorking = false;

    m_worker.join();

    for (const auto &provider : m_config.providers()) {
        mosquitto_unsubscribe(m_mosq, NULL, provider.id().c_str());
    }
}

void Reciever::backgroundWork()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    for (const auto &provider : m_config.providers()) {
        mosquitto_subscribe(m_mosq, NULL, provider.id().c_str(), 0);
    }

    while (m_isWorking) {
        mosquitto_loop(m_mosq, -1, 1);
    }
}

}
}
