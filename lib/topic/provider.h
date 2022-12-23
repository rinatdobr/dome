#ifndef TOPIC_PROVIDER_H
#define TOPIC_PROVIDER_H

#include <vector>

#include "config/provider.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "processor.h"

namespace dome {
namespace topic {

class Provider : public Processor
{
public:
    Provider(const std::vector<dome::config::Provider> &providers, std::vector<dome::message::Processor*> &messageProcessors);
    ~Provider();

private:
    const std::vector<dome::config::Provider> &m_providers;
    std::vector<dome::message::Processor*> m_messageProcessors;

    std::vector<std::string> topics() override;
    void process(dome::mosq::Mosquitto &mosq, const std::string &topic, nlohmann::json &jMessage) override;
    void sendPingMessage(dome::mosq::Mosquitto &mosq, const std::string &message) override;
};

}
}

#endif
