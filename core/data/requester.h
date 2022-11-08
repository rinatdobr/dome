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
    Info
};

    Request::Type type;
    nlohmann::json reply;
};

class Info : public Request
{
public:
    std::string idFrom;
    std::vector<std::string> args;
    std::unordered_map<std::string, bool> sources;
};

class Requester : public Processor
{
public:
    explicit Requester(const std::vector<dome::config::Provider> &providers);
    ~Requester();

    void process(const dome::config::Provider &provider, nlohmann::json &jMessage) override;

private:
    const std::vector<dome::config::Provider> &m_providers;
    std::deque<std::shared_ptr<Request>> m_requests;
    dome::mosq::Mosquitto m_mosq;
};

}
}

#endif
