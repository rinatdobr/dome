#ifndef MESSAGE_STATISTIC_H
#define MESSAGE_STATISTIC_H

#include <string>

#include "processor.h"
#include "mosquitto/mosq.h"
#include "message.h"

namespace dome {
namespace message {

class StatisticMessage : public Message
{
public:
};

class Statistic : public Processor
{
public:
    explicit Statistic(const std::string &dbPath, const std::vector<dome::config::Provider> &providers);
    ~Statistic();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    std::string m_dbPath;
    const std::vector<dome::config::Provider> &m_providers;
};

}
}

#endif
