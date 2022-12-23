#ifndef TOPIC_REQUEST_H
#define TOPIC_REQUEST_H

#include <vector>

#include "config/provider.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "processor.h"

namespace dome {
namespace topic {

class Request : public Processor
{
public:
    Request(const dome::config::Provider &provider, std::vector<dome::message::Processor*> &messageProcessors);
    ~Request();

private:
    const dome::config::Provider &m_provider;
    std::vector<dome::message::Processor*> m_messageProcessors;

    std::vector<std::string> topics() override;
    void process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage) override;
    void sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message) override;
};

}
}

#endif
