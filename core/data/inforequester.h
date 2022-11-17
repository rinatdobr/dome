#ifndef DATA_INFO_REQUESTER_H
#define DATA_INFO_REQUESTER_H

#include <string>
#include <unordered_map>
#include <deque>

#include <config/provider.h>
#include <data/processor.h>
#include <mosquitto/mosq.h>
#include "requester.h"

namespace dome {
namespace data {

class Info : public Request
{
public:
    std::unordered_map<std::string, bool> sources;
};

class InfoRequester : public Processor
{
public:
    explicit InfoRequester(const std::vector<dome::config::Provider> &providers);
    ~InfoRequester();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    std::deque<std::shared_ptr<Request>> m_requests;
};

}
}

#endif
