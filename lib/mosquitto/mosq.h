#ifndef MOSQUITTO_MOSQ_H
#define MOSQUITTO_MOSQ_H

#include <mosquitto.h>
#include <string>

namespace dome {
namespace mosq {

class Mosquitto
{
public:
    explicit Mosquitto(const std::string clientId, void *owner);
    ~Mosquitto();

    struct mosquitto *mosq();

private:
    struct mosquitto *m_mosq;
};

}
}

#endif
