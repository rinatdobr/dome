#ifndef REPLIER_H
#define REPLIER_H

#include <message/processor.h>
#include "tdclient.h"

namespace dome {
namespace message {

class Reply : public Processor
{
public:
    explicit Reply(dome::data::TdClient &tdClient);
    ~Reply();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    dome::data::TdClient &m_tdClient;
};

}
}

#endif
