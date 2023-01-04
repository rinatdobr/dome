#include "ipcamera.h"

#include <spdlog/spdlog.h>

#include <set>
#include "utils/utils.h"
#include "message/message.h"

namespace dome {
namespace message {

IpCamera::IpCamera(const std::vector<dome::config::EndPoint> &endPointConfigs, const std::vector<dome::config::IpCamera> &ipCameras)
    : m_endPointConfigs(endPointConfigs)
    , m_ipCameras(ipCameras)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

IpCamera::~IpCamera()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void IpCamera::process(dome::mosq::Mosquitto &mosq, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
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
        if (dome::message::command::IpCamera != name) {
            spdlog::trace("ignore...");
            return;
        }
        if (!CheckJsonMessageForKeys(jSource, { "message_id", "chat_id" })) return;

        auto args = parseArgs(std::string(body, delimetr + 1));
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

        auto reply = std::make_shared<IpCameraReply>();
        reply->idFrom = endPointConfig.id();
        reply->body["type"] = dome::message::type::Reply;
        reply->body["reply"] = dome::message::command::IpCamera;
        reply->body["message_id"] = jSource["message_id"];
        reply->body["chat_id"] = jSource["chat_id"];
        reply->body["quality"] = quality;

        std::set<std::string> endPointIds;
        for (const auto &endPointConfig : m_endPointConfigs) {
            for (const auto &source : endPointConfig.sources()) {
                if (source.type() == dome::config::Source::Type::IpCamera) {
                    for (const auto &ipCamera : m_ipCameras) {
                        if (ipCamera.id() == source.id() &&
                            ipCamera.name() == ipCameraName) {
                            reply->sources[source.id()] = false;
                            endPointIds.insert(endPointConfig.id());
                        }
                    }
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

        auto reply = static_cast<IpCameraReply*>(request.get());
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
            std::string data = reply->body.dump();
            spdlog::debug("replying to \"{}\" from \"{}\"...", GetReplyTopic(reply->idFrom), mosq.clientId());
            mosq.publish(GetReplyTopic(reply->idFrom), data);
            m_replies.pop_front();
        }
    }
}

}
}
