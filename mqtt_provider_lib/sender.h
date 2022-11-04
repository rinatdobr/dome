#ifndef SENDER_H
#define SENDER_H

#include <mosquitto.h>

#include <config/provider.h>
#include "provider.h"

namespace dome {
namespace mqtt_provider {

class Sender
{
public:
    explicit Sender(const dome::config::Provider &config, dome::mqtt_provider::Provider &provider);
    ~Sender();

    void start(int period);
private:
    const dome::config::Provider &m_config;
    struct mosquitto *m_mosq;
    std::string m_topic;
    dome::mqtt_provider::Provider &m_provider;
};

}
}

#endif
