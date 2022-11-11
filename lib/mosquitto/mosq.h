#ifndef MOSQUITTO_MOSQ_H
#define MOSQUITTO_MOSQ_H

#include <mosquitto.h>
#include <string>

namespace dome {
namespace mosq {

const int KeepAliveSec = 3600;

class Mosquitto
{
public:
    explicit Mosquitto(const std::string clientId, void *owner);
    ~Mosquitto();

    struct mosquitto *mosq();
    const std::string &clientId() const;
    int keepAliveSec();
    bool decrementKeepAlive(int diffSec = 1);
    static std::string PingMessage();

private:
    struct mosquitto *m_mosq;
    std::string m_clientId;
    int m_keepAliveSec;
};

}
}

#endif
