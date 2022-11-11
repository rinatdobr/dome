#ifndef DATA_GETTER_H
#define DATA_GETTER_H

#include "data/processor.h"
#include "mosquitto/sender.h"

namespace dome {
namespace data {

class Getter : public Processor
{
public:
    explicit Getter(dome::mosq::Sender::Trigger &senderTrigger);
    ~Getter();

    void process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    dome::mosq::Sender::Trigger &m_senderTrigger;
};

}
}

#endif
