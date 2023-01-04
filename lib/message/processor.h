#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <nlohmann/json.hpp>

#include "config/endpoint.h"
#include "mosquitto/mosq.h"

namespace dome {
namespace message {

class Processor
{
public:
    virtual void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) = 0;
};

}
}

#endif
