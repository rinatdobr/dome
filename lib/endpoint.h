#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <mosquitto.h>
#include <functional>
#include <nlohmann/json.hpp>

#include "config/endpoint.h"
#include "data/provider.h"
#include "utils/validatable.h"

#include "message/processor.h"
#include "message/request/get.h"
#include "mosquitto/sender.h"
#include "mosquitto/receiver.h"

namespace dome {
namespace endpoint {

class EndPoint : public dome::utils::Validatable
{
public:
    EndPoint(dome::config::EndPoint &endPointConfig, dome::data::Provider &dataProvider);
    ~EndPoint();

    void start();
private:
    dome::config::EndPoint &m_endPointConfig;
    dome::data::Provider &m_dataProvider;
    dome::mosq::Sender::Trigger m_trigger;
    dome::mosq::Sender *m_sender;
    dome::mosq::Receiver *m_receiver;
    std::vector<dome::message::Processor*> m_processors;
};

}
}

#endif
