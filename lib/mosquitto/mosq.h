#ifndef MOSQUITTO_MOSQ_H
#define MOSQUITTO_MOSQ_H

#include <mosquitto.h>
#include <vector>
#include <string>

#include "utils/validatable.h"

namespace dome {
namespace mosq {

const int KeepAliveSec = 3600;

class Mosquitto : public dome::utils::Validatable
{
public:
    explicit Mosquitto(const std::string &clientId, std::vector<std::string> topics, void *owner);
    ~Mosquitto();

    const std::string &clientId() const;
    int keepAliveSec();
    bool decrementKeepAlive(int diffSec = 1);
    void setCallback(void (*messageCallback)(mosquitto *, void *, const mosquitto_message *));
    void publish(const std::string &topic, const std::string &message);

private:
    struct mosquitto *m_mosq;
    std::string m_clientId;
    std::vector<std::string> m_topics;
    int m_keepAliveSec;

    void reconnect();
    bool connect();
    void disconnect();
    bool subscribe();
    void unsubscribe();
};

}
}

#endif
