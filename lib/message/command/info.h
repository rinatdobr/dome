#ifndef MESSAGE_INFO_H
#define MESSAGE_INFO_H

#include <string>
#include <unordered_map>
#include <deque>

#include "config/endpoint.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "command.h"
#include "reply.h"

namespace dome {
namespace message {

class InfoReply : public Reply
{
public:
    std::unordered_map<std::string, bool> sources;
};

class Info : public Processor, public Command
{
public:
    explicit Info(const std::vector<dome::config::EndPoint> &endPointConfigs);
    ~Info();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::EndPoint> &m_endPointConfigs;
    std::deque<std::shared_ptr<Reply>> m_replies;
};

}
}

#endif
