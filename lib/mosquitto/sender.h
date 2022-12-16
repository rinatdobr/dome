#ifndef MOSQUITTO_SENDER_H
#define MOSQUITTO_SENDER_H

#include <thread>
#include <mutex>
#include <condition_variable>

#include "utils/threader.h"
#include "config/provider.h"
#include "data/provider.h"
#include "mosq.h"
#include "utils/validatable.h"

namespace dome {
namespace mosq {

class Sender : public dome::utils::Threader, public dome::utils::Validatable
{
public:

class Trigger
{
public:
    std::mutex mutex;
    std::condition_variable cv;
};

    explicit Sender(const std::string &mosqClientId, const dome::config::Provider &config, dome::data::Provider &provider, Trigger &trigger);
    ~Sender();

    Trigger &trigger();

private:
    const dome::config::Provider &m_config;
    dome::data::Provider &m_provider;
    dome::mosq::Mosquitto m_mosq;
    Trigger &m_trigger;

    void backgroundWork() override;
};

}
}

#endif
