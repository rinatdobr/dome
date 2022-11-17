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
    Statistic,
    IpCamera
};

    std::string idFrom;
    Request::Type type;
    nlohmann::json reply;
    std::vector<std::string> args;
};

std::vector<std::string> ParseArgs(const std::string &args);

}
}

#endif
