#ifndef MESSAGE_INFO_H
#define MESSAGE_INFO_H

#include <string>
#include <unordered_map>
#include <deque>

#include "config/provider.h"
#include "processor.h"
#include "mosquitto/mosq.h"
#include "message.h"

namespace dome {
namespace message {

class InfoMessage : public Message
{
public:
    std::unordered_map<std::string, bool> sources;
};

class Info : public Processor
{
public:
    explicit Info(const std::vector<dome::config::Provider> &providers);
    ~Info();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    std::deque<std::shared_ptr<Message>> m_messages;
};

}
}

#endif
