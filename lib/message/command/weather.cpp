#include "weather.h"

#include <spdlog/spdlog.h>

#include <set>
#include "utils/utils.h"
#include "message/message.h"

namespace dome {
namespace message {

const std::string WeatherRequestStr("/weather");

Weather::Weather(const dome::config::EndPoint &endPointConfig)
    : m_endPointConfig(endPointConfig)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Weather::~Weather()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Weather::process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] == dome::message::type::Command) {
        spdlog::debug("command processing...");

        if (!CheckJsonMessageForKeys(jMessage, { endPointConfig.sources()[0].id() })) return;

        nlohmann::json jSource = nlohmann::json::parse(jMessage[endPointConfig.sources()[0].id()].get<std::string>());
        if (!CheckJsonMessageForKeys(jSource, { "body" })) return;

        std::string body = jSource["body"].get<std::string>();
        std::size_t delimetr = body.find(' ');
        std::string name(body, 0, delimetr);
        if (WeatherRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = parseArgs(std::string(body, delimetr + 1));
        if (args.size() != 1 && args.size() != 2) {
            spdlog::error("Invalid number of args");
            return;
        }
        std::string forecastPeriod;
        // 0 - forecast period
        if (args.size() == 1) {
            forecastPeriod = args[0];
        }

        auto reply = std::make_shared<WeatherReply>();
        reply->idFrom = endPointConfig.id();
        reply->body["type"] = dome::message::type::Reply;
        reply->body["reply"] = dome::message::command::Statistic;
        reply->body["message_id"] = jSource["message_id"];
        reply->body["chat_id"] = jSource["chat_id"];
        reply->body["forecast_period"] = forecastPeriod;
        m_replies.push_back(reply);

        nlohmann::json jData;
        jData["type"] = dome::message::type::Request;
        jData["request"] = dome::message::request::Get;
        std::string data = jData.dump();
        spdlog::debug("requesting \"exec\" on \"{}\" from \"{}\"...", GetExecTopic(endPointConfig.id()), mosq.clientId());
        mosq.publish(GetExecTopic(endPointConfig.id()), data);
    }
    else if (jMessage["type"] == dome::message::type::Data) {
        if (m_replies.size() == 0) {
            spdlog::debug("no reply to fill");
            return;
        }
        auto request = m_replies.front();
        spdlog::debug("reply filling...");

        auto reply = static_cast<WeatherMessage*>(request.get());
        for (const auto &source : endPointConfig.sources()) {
            if (!reply->sources.count(source.id())) {
                continue;
            }
            else if (!reply->sources[source.id()]) {
                if (!CheckJsonMessageForKeys(jMessage, { source.id() })) continue;
                reply->body["path"] = jMessage[source.id()];
                reply->sources[source.id()] = true;
            }
        }

        bool gotAllData = true;
        for (const auto &source : reply->sources) {
            gotAllData &= source.second;
            if (!gotAllData) {
                break;
            }
        }

        if (gotAllData) {
            std::string data = reply->reply.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(reply->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(reply->idFrom), data);
            m_replies.pop_front();
        }
    }
}

}
}
