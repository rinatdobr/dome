#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <nlohmann/json.hpp>

#include "config/provider.h"
#include "mosquitto/mosq.h"

namespace dome {
namespace data {

class Processor
{
public:
    virtual void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) = 0;
};

}
}

#endif
