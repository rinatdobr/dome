#include "info.h"

#include <spdlog/spdlog.h>

#include <set>

#include "utils/utils.h"

namespace dome {
namespace message {

const std::string InfoRequestStr("/info");

Info::Info(const std::vector<dome::config::Provider> &providers)
    : m_providers(providers)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Info::~Info()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Info::process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] == "request") {
        spdlog::debug("request processing...");

        if (!CheckJsonMessageForKeys(jMessage, { provider.sources()[0].id })) return;

        nlohmann::json jSource = nlohmann::json::parse(jMessage[provider.sources()[0].id].get<std::string>());
        if (!CheckJsonMessageForKeys(jSource, { "request" })) return;

        std::string text = jSource["request"].get<std::string>();
        std::size_t delimetr = text.find(' ');
        std::string name(text, 0, delimetr);
        if (InfoRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = ParseArgs(std::string(text, delimetr + 1));
        auto info = std::make_shared<InfoMessage>();
        info->type = Message::Type::Info;
        info->idFrom = provider.id();
        info->args = args;
        info->reply["type"] = InfoRequestStr;
        info->reply["message_id"] = jSource["message_id"];
        info->reply["chat_id"] = jSource["chat_id"];
        std::set<std::string> providerIds;
        for (const auto &provider : m_providers) {
            for (const auto &source : provider.sources()) {
                if (source.type == dome::config::Source::Type::Temperature ||
                    source.type == dome::config::Source::Type::Humidity ||
                    source.type == dome::config::Source::Type::Co2) {
                    info->sources[source.id] = false;
                    providerIds.insert(provider.id());
                }
            }
        }
        m_messages.push_back(info);

        nlohmann::json jData;
        jData["type"] = "request";
        jData["request"] = "get";
        std::string data = jData.dump();
        for (const auto &providerId : providerIds) {
            spdlog::debug("requesting \"get\" on \"{}\" from \"{}\"...", GetRequestTopic(providerId), mosq.clientId());
            mosq.publish(GetRequestTopic(providerId), data);
        }
    }
    else if (jMessage["type"] == "data") {
        if (m_messages.size() == 0) {
            spdlog::debug("no request to fill");
            return;
        }
        auto request = m_messages.front();
        spdlog::debug("request filling...");

        auto info = static_cast<InfoMessage*>(request.get());
        for (const auto &source : provider.sources()) {
            if (!info->sources.count(source.id)) {
                continue;
            }
            else if (!info->sources[source.id]) {
                if (!CheckJsonMessageForKeys(jMessage, { source.id })) continue;
                info->reply["data"][provider.location()][dome::config::Source::TypeToStr(source.type)] = jMessage[source.id];
                info->sources[source.id] = true;
            }
        }

        bool gotAllData = true;
        for (const auto &source : info->sources) {
            gotAllData &= source.second;
            if (!gotAllData) {
                break;
            }
        }

        if (gotAllData) {
            std::string data = info->reply.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(info->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(info->idFrom), data);
            m_messages.pop_front();
        }
    }
}

}
}
