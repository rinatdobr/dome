#include "inforequester.h"

#include <spdlog/spdlog.h>

#include <set>

#include <utils.h>

namespace dome {
namespace data {

const std::string InfoRequestStr("/info");

InfoRequester::InfoRequester(const std::vector<dome::config::Provider> &providers)
    : m_providers(providers)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

InfoRequester::~InfoRequester()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void InfoRequester::process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["type"] == "request") {
        spdlog::debug("request processing...");
        nlohmann::json jSource = nlohmann::json::parse(jMessage[provider.sources()[0].id].get<std::string>());
        std::string text = jSource["request"].get<std::string>();
        std::size_t delimetr = text.find(' ');
        std::string name(text, 0, delimetr);
        if (InfoRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }

        auto args = ParseArgs(std::string(text, delimetr + 1));

        auto info = std::make_shared<Info>();
        info->type = Request::Type::Info;
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
        m_requests.push_back(info);

        nlohmann::json jData;
        jData["name"] = "get";
        std::string data = jData.dump();
        for (const auto &providerId : providerIds) {
            spdlog::debug("requesting \"get\" from {}...", providerId);
            int res = mosquitto_publish(mosq.mosq(), nullptr, GetRequestTopic(providerId).c_str(), data.size(), data.c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                spdlog::error("mosquitto_publish to \"{}\" error[{}]: {}", GetRequestTopic(providerId), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
        }
    }
    else if (jMessage["type"] == "data") {
        if (m_requests.size() == 0) {
            spdlog::debug("no request to fill");
            return;
        }
        auto request = m_requests.front();
        spdlog::debug("request filling...");

        auto info = static_cast<Info*>(request.get());
        for (const auto &source : provider.sources()) {
            if (!info->sources.count(source.id)) {
                continue;
            }
            else if (!info->sources[source.id]) {
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
            spdlog::debug("replying...");
            int res = mosquitto_publish(mosq.mosq(), nullptr, GetReplyTopic(info->idFrom).c_str(), data.size(), data.c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                spdlog::error("mosquitto_publish to \"{}\" error[{}]: {}", GetReplyTopic(provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
            m_requests.pop_front();
        }
    }
}

}
}
