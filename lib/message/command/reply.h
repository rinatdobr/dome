#ifndef MESSAGE_COMMAND_REPLY_H
#define MESSAGE_COMMAND_REPLY_H

#include <string>
#include <deque>
#include <unordered_map>
#include <memory>

#include "config/endpoint.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"

namespace dome {
namespace message {

class Reply
{
public:
    std::string idFrom;
    nlohmann::json body;
};

}
}

#endif
