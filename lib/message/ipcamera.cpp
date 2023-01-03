#include "ipcamera.h"

#include <spdlog/spdlog.h>

#include <set>
#include "utils/utils.h"

namespace dome {
namespace message {

const std::string IpCameraRequestStr("/ipcamera");

IpCamera::IpCamera(const std::vector<dome::config::Provider> &providers, const std::vector<dome::config::IpCamera> &ipCameras)
    : m_providers(providers)
    , m_ipCameras(ipCameras)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

IpCamera::~IpCamera()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void IpCamera::process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) return;

    if (jMessage["type"] == "request") {
        spdlog::debug("request processing...");

        if (!CheckJsonMessageForKeys(jMessage, { provider.sources()[0].id })) return;

        nlohmann::json jSource = nlohmann::json::parse(jMessage[provider.sources()[0].id].get<std::string>());
        if (!CheckJsonMessageForKeys(jSource, { "body" })) return;

        std::string body = jSource["body"].get<std::string>();
        std::size_t delimetr = body.find(' ');
        std::string name(body, 0, delimetr);
        if (IpCameraRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = ParseArgs(std::string(body, delimetr + 1));
        if (args.size() != 1 && args.size() != 2) {
            spdlog::error("Invalid number of args");
            return;
        }
        std::string ipCameraName;
        std::string quality = "sd";
        // 0 - ip camera name
        if (args.size() == 1) {
            ipCameraName = args[0];
        }
        // 0 - ip camera name
        // 1 - quality
        else if (args.size() == 2) {
            ipCameraName = args[0];
            quality = args[1];
        }

        auto ipCameraRequest = std::make_shared<IpCameraMessage>();
        ipCameraRequest->type = Message::Type::IpCamera;
        ipCameraRequest->idFrom = provider.id();
        ipCameraRequest->args = args;
        ipCameraRequest->reply["type"] = IpCameraRequestStr;
        ipCameraRequest->reply["message_id"] = jSource["message_id"];
        ipCameraRequest->reply["chat_id"] = jSource["chat_id"];
        ipCameraRequest->reply["quality"] = quality;
        std::set<std::string> providerIds;
        for (const auto &provider : m_providers) {
            for (const auto &source : provider.sources()) {
                if (source.type == dome::config::Source::Type::IpCamera) {
                    for (const auto &ipCamera : m_ipCameras) {
                        if (ipCamera.id() == source.id &&
                            ipCamera.name() == ipCameraName) {
                            ipCameraRequest->sources[source.id] = false;
                            providerIds.insert(provider.id());
                        }
                    }
                }
            }
        }
        m_messages.push_back(ipCameraRequest);

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

        auto ipCameraRequest = static_cast<IpCameraMessage*>(request.get());
        for (const auto &source : provider.sources()) {
            if (!ipCameraRequest->sources.count(source.id)) {
                continue;
            }
            else if (!ipCameraRequest->sources[source.id]) {
                if (!CheckJsonMessageForKeys(jMessage, { source.id })) continue;
                ipCameraRequest->reply["path"] = jMessage[source.id];
                ipCameraRequest->sources[source.id] = true;
            }
        }

        bool gotAllData = true;
        for (const auto &source : ipCameraRequest->sources) {
            gotAllData &= source.second;
            if (!gotAllData) {
                break;
            }
        }

        if (gotAllData) {
            std::string data = ipCameraRequest->reply.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(ipCameraRequest->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(ipCameraRequest->idFrom), data);
            m_messages.pop_front();
        }
    }
}

}
}
