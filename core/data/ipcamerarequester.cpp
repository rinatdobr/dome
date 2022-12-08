#include "ipcamerarequester.h"

#include <spdlog/spdlog.h>

#include <set>
#include <utils.h>

namespace dome {
namespace data {

const std::string IpCameraRequestStr("/ipcamera");

IpCameraRequester::IpCameraRequester(const std::vector<dome::config::Provider> &providers, const std::vector<dome::config::IpCamera> &ipCameras)
    : m_providers(providers)
    , m_ipCameras(ipCameras)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

IpCameraRequester::~IpCameraRequester()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void IpCameraRequester::process(dome::mosq::Mosquitto &mosq, const dome::config::Provider &provider, nlohmann::json &jMessage)
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
        if (IpCameraRequestStr != name) {
            spdlog::trace("ignore...");
            return;
        }

        auto args = ParseArgs(std::string(text, delimetr + 1));
        if (args.size() != 1 && args.size() != 2) {
            spdlog::error("invalid number of args");
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

        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto ipCameraRequest = std::make_shared<IpCamera>();
        ipCameraRequest->type = Request::Type::IpCamera;
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
        m_requests.push_back(ipCameraRequest);

        nlohmann::json jData;
        jData["request"] = "get";
        std::string data = jData.dump();
        for (const auto &providerId : providerIds) {
            spdlog::debug("requesting \"get\"...");
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

        auto ipCameraRequest = static_cast<IpCamera*>(request.get());
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
            spdlog::debug("replying...");
            int res = mosquitto_publish(mosq.mosq(), nullptr, GetReplyTopic(ipCameraRequest->idFrom).c_str(), data.size(), data.c_str(), 0, false);
            if (res != MOSQ_ERR_SUCCESS) {
                spdlog::error("mosquitto_publish to \"{}\" error[{}]: {}", GetReplyTopic(provider.id()), res, res == MOSQ_ERR_ERRNO ? std::strerror(errno) : mosquitto_strerror(res));
            }
            m_requests.pop_front();
        }
    }
}

}
}
