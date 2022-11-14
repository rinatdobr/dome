#ifndef DATA_REQUESTER_H
#define DATA_REQUESTER_H

#include <string>
#include <deque>
#include <unordered_map>
#include <memory>

#include <config/provider.h>
#include <data/processor.h>
#include <mosquitto/mosq.h>

namespace dome {
namespace data {

class Request
{
public:
enum class Type
{
    Info,
    Statistic
};

    std::string idFrom;
    Request::Type type;
    nlohmann::json reply;
    std::vector<std::string> args;
};

class Info : public Request
{
public:
    std::unordered_map<std::string, bool> sources;
};

class Statistic : public Request
{
public:
};

class Requester : public Processor
{
public:
    explicit Requester(const std::vector<dome::config::Provider> &providers);
    ~Requester();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    std::deque<std::shared_ptr<Request>> m_requests;
};

}
}

#endif
