#ifndef MESSAGE_GET_H
#define MESSAGE_GET_H

#include "message/processor.h"
#include "mosquitto/sender.h"

namespace dome {
namespace message {

class Get : public Processor
{
public:
    explicit Get(dome::mosq::Sender::Trigger &senderTrigger);
    ~Get();

    void process(dome::mosq::Mosquitto &, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    dome::mosq::Sender::Trigger &m_senderTrigger;
};

}
}

#endif
