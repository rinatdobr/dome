#ifndef MESSAGE_STATISTIC_H
#define MESSAGE_STATISTIC_H

#include <string>

#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "command.h"
#include "reply.h"

namespace dome {
namespace message {

class StatisticReply : public Reply
{
};

class Statistic : public Processor, public Command
{
public:
    explicit Statistic(const std::string &dbPath, const std::vector<dome::config::EndPoint> &endPointConfigs);
    ~Statistic();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    std::string m_dbPath;
    const std::vector<dome::config::EndPoint> &m_endPointConfigs;
};

}
}

#endif
