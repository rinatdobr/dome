#include "requester.h"

#include <spdlog/spdlog.h>

#include <utils.h>

namespace dome {
namespace data {

const std::string InfoRequestStr("/info");

std::vector<std::string> ParseArgs(const std::string &args)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (args.size() == 0) {
        return {};
    }

    std::vector<std::string> result;

    std::size_t start = 0;
    int i = 0;
    while (start != std::string::npos) {
        std::size_t end = args.find(' ', start);
        if (end != std::string::npos) {
            result.push_back(std::string(args, start, end - start));
            spdlog::debug("arg[{}]={}", i++, result.back());
        }
        else {
            result.push_back(std::string(args, start));
            spdlog::debug("arg[{}]={}", i++, result.back());
            break;
        }
        start = end + 1;
    }

    return result;
}

Requester::Requester(const std::vector<dome::config::Provider> &providers)
    : m_providers(providers)
    , m_mosq("", this)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Requester::~Requester()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Requester::process(const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["type"] == "request") {
        spdlog::debug("request processing...");
        nlohmann::json jSource = nlohmann::json::parse(jMessage[provider.sources()[0].id].get<std::string>());
        std::string text = jSource["request"].get<std::string>();
        std::size_t delimetr = text.find(' ');
        std::string name(text, 0, delimetr);
        auto args = ParseArgs(std::string(text, delimetr + 1));

        if (InfoRequestStr == name) {
            auto info = std::make_shared<Info>();
            info->type = Request::Type::Info;
            info->idFrom = provider.id();
            info->args = args;
            info->reply["type"] = "/info";
            info->reply["message_id"] = jSource["message_id"];
            info->reply["chat_id"] = jSource["chat_id"];
            for (const auto &provider : m_providers) {
                for (const auto &source : provider.sources()) {
                    if (source.type == dome::config::Source::Type::Temperature ||
                        source.type == dome::config::Source::Type::Humidity) {
                        info->sources[source.id] = false;
                    }
                }
            }
            m_requests.push_back(info);

            nlohmann::json jData;
            jData["name"] = "ping";
            std::string data = jData.dump();
            for (const auto &provider : m_providers) {
                mosquitto_publish(m_mosq.mosq(), nullptr, GetRequestTopic(provider.id()).c_str(), data.size(), data.c_str(), 0, false);
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

        switch (request->type) {
            case Request::Type::Info:
                auto info = static_cast<Info*>(request.get());
                for (const auto &source : provider.sources()) {
                    if (!info->sources.count(source.id)) {
                        continue;
                    }
                    else if (!info->sources[source.id]) {
                        info->reply["data"][source.location][dome::config::Source::TypeToStr(source.type)] = jMessage[source.id];
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
                    mosquitto_publish(m_mosq.mosq(), nullptr, GetReplyTopic(info->idFrom).c_str(), data.size(), data.c_str(), 0, false);
                    m_requests.pop_front();
                }
            break;
        }
    }
}

}
}
