#ifndef TOPIC_PROCESSOR_H
#define TOPIC_PROCESSOR_H

#include "mosquitto/mosq.h"

#include <string>
#include <nlohmann/json.hpp>

namespace dome {
namespace topic {

class Processor
{
public:
    virtual void subscribe(dome::mosq::Mosquitto &mosq) = 0;
    virtual void unsubscribe(dome::mosq::Mosquitto &mosq) = 0;
    virtual void process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage) = 0;
    virtual void sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message) = 0;
};

}
}

#endif
