#ifndef MOSQUITTO_MOSQ_H
#define MOSQUITTO_MOSQ_H

#include <mosquitto.h>
#include <string>

#include "utils/validatable.h"

namespace dome {
namespace mosq {

const int KeepAliveSec = 3600;

class Mosquitto : public dome::utils::Validatable
{
public:
    explicit Mosquitto(const std::string clientId, void *owner);
    ~Mosquitto();

    struct mosquitto *mosq();
    const std::string &clientId() const;
    int keepAliveSec();
    bool decrementKeepAlive(int diffSec = 1);

private:
    struct mosquitto *m_mosq;
    std::string m_clientId;
    int m_keepAliveSec;
};

}
}

#endif
