#include "info.h"

#include <spdlog/spdlog.h>

#include <set>

#include "utils/utils.h"
#include "message/message.h"

namespace dome {
namespace message {

Info::Info(const std::vector<dome::config::EndPoint> &endPointConfigs)
    : m_endPointConfigs(endPointConfigs)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Info::~Info()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Info::process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
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
        if (dome::message::command::Info != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = parseArgs(std::string(body, delimetr + 1));
        auto reply = std::make_shared<InfoReply>();
        reply->idFrom = endPointConfig.id();
        reply->body["type"] = dome::message::type::Reply;
        reply->body["reply"] = dome::message::command::Info;
        reply->body["message_id"] = jSource["message_id"];
        reply->body["chat_id"] = jSource["chat_id"];

        std::set<std::string> endPointIds;
        for (const auto &endPointConfig : m_endPointConfigs) {
            for (const auto &source : endPointConfig.sources()) {
                if (source.type() == dome::config::Source::Type::Temperature ||
                    source.type() == dome::config::Source::Type::Humidity ||
                    source.type() == dome::config::Source::Type::Co2) {
                    reply->sources[source.id()] = false;
                    endPointIds.insert(endPointConfig.id());
                }
            }
        }
        m_replies.push_back(reply);

        nlohmann::json jData;
        jData["type"] = dome::message::type::Request;
        jData["request"] = dome::message::request::Get;
        std::string data = jData.dump();
        for (const auto &endPointId : endPointIds) {
            spdlog::debug("requesting \"get\" on \"{}\" from \"{}\"...", GetRequestTopic(endPointId), mosq.clientId());
            mosq.publish(GetRequestTopic(endPointId), data);
        }
    }
    else if (jMessage["type"] == dome::message::type::Data) {
        if (m_replies.size() == 0) {
            spdlog::debug("no reply to fill");
            return;
        }
        auto request = m_replies.front();
        spdlog::debug("reply filling...");

        auto reply = static_cast<InfoReply*>(request.get());
        for (const auto &source : endPointConfig.sources()) {
            if (!reply->sources.count(source.id())) {
                continue;
            }
            else if (!reply->sources[source.id()]) {
                if (!CheckJsonMessageForKeys(jMessage, { source.id() })) continue;
                reply->body["data"][endPointConfig.location()][dome::config::Source::TypeToStr(source.type())] = jMessage[source.id()];
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
            std::string data = reply->body.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(reply->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(reply->idFrom), data);
            m_replies.pop_front();
        }
    }
}

}
}
