#ifndef MESSAGE_MESSAGE_H
#define MESSAGE_MESSAGE_H

#include <string>

namespace dome {
namespace message {

namespace type {

const static std::string Data = "data";
const static std::string Command = "command";
const static std::string Request = "request";
const static std::string Reply = "reply";
const static std::string Ping = "ping";

}

namespace command {

const static std::string Info = "/info";
const static std::string Statistic = "/statistic";
const static std::string IpCamera = "/ipcamera";

}

namespace request {

const static std::string Get = "get";

}

class Message
{
};

}
}

#endif
