#ifndef SENDER_H
#define SENDER_H

#include <mosquitto.h>

#include "config/config.h"
#include "provider.h"

namespace dome {
namespace mqtt {

class Sender
{
public:
    explicit Sender(const dome::config::Config &config, dome::data::Provider &provider);
    ~Sender();

    void start(int period);
private:
    struct mosquitto *m_mosq;
    std::string m_topic;
    dome::data::Provider &m_provider;
};

}
}

#endif
