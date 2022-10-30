#ifndef SENDER_H
#define SENDER_H

#include <mosquitto.h>

#include "config/config.h"
#include "provider.h"

namespace dome {
namespace core {

class Sender
{
public:
    explicit Sender(const dome::config::Config &config, dome::core::Provider &provider);
    ~Sender();

    void start(int period);
private:
    const dome::config::Config &m_config;
    struct mosquitto *m_mosq;
    std::string m_topic;
    dome::core::Provider &m_provider;
};

}
}

#endif
