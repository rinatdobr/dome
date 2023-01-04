#ifndef MESSAGE_WEATHER_H
#define MESSAGE_WEATHER_H

#include <string>
#include <unordered_map>
#include <deque>

#include "config/openweather.h"
#include "config/endpoint.h"
#include "message/processor.h"
#include "mosquitto/mosq.h"
#include "command.h"
#include "reply.h"

namespace dome {
namespace message {

class WeatherReply : public Reply
{
public:
    std::unordered_map<std::string, bool> sources;
};

class Weather : public Processor, public Command
{
public:
    explicit Weather(const dome::config::EndPoint &endPointConfig);
    ~Weather();

    void process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage) override;

private:
    const dome::config::EndPoint &m_endPointConfig;
    std::deque<std::shared_ptr<Reply>> m_replies;
};

}
}

#endif
