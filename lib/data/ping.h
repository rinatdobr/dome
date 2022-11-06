#ifndef DATA_PING_H
#define DATA_PING_H

#include "data/processor.h"
#include "mosquitto/sender.h"

namespace dome {
namespace data {

class Ping : public Processor
{
public:
    explicit Ping(dome::mosq::Sender::Trigger &senderTrigger);
    ~Ping();

    void process(const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    dome::mosq::Sender::Trigger &m_senderTrigger;
};

}
}

#endif
