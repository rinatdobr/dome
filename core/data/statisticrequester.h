#ifndef DATA_STATISTIC_REQUESTER_H
#define DATA_STATISTIC_REQUESTER_H

#include <string>

#include <data/processor.h>
#include <mosquitto/mosq.h>
#include "requester.h"

namespace dome {
namespace data {

class Statistic : public Request
{
public:
};

class StatisticRequester : public Processor
{
public:
    explicit StatisticRequester(const std::string &dbPath, const std::vector<dome::config::Provider> &providers);
    ~StatisticRequester();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    std::string m_dbPath;
    const std::vector<dome::config::Provider> &m_providers;
};

}
}

#endif
