#ifndef MESSAGE_MESSAGE_H
#define MESSAGE_MESSAGE_H

#include <string>
#include <deque>
#include <unordered_map>
#include <memory>

#include "config/provider.h"
#include "processor.h"
#include "mosquitto/mosq.h"

namespace dome {
namespace message {

class Message
{
public:
enum class Type
{
    Info,
    Statistic,
    IpCamera,
    Weather
};

    std::string idFrom;
    Message::Type type;
    nlohmann::json reply;
    std::vector<std::string> args;
};

std::vector<std::string> ParseArgs(const std::string &args);

}
}

#endif
