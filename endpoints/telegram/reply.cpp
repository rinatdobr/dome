#include "reply.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include <message/message.h>
#include <config/endpoint.h>
#include <utils/utils.h>

namespace dome {
namespace message {

Reply::Reply(dome::data::TdClient &tdClient)
    : m_tdClient(tdClient)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Reply::~Reply()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Reply::process(dome::mosq::Mosquitto &, const dome::config::EndPoint &endPointConfig, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type", "reply" })) {
        return;
    };

    if (jMessage["type"] != dome::message::type::Reply) {
        spdlog::debug("ignoring...");
        return;
    }

    if (jMessage["reply"] == dome::message::command::Info) {
        spdlog::debug("reply processing...");

        if (!CheckJsonMessageForKeys(jMessage, { "data", "chat_id", "message_id" })) return;

        std::ostringstream reply;
        for (const auto &location : jMessage["data"].items()) {
            reply << location.key() << ":";
            for (const auto &data : location.value().items()) {
                if (data.key() == "temperature") {
                    reply << "\n  Температура: " << std::setprecision(3) << data.value().get<double>();
                }
                else if (data.key() == "humidity") {
                    reply << "\n  Влажность: " << std::setprecision(3) << data.value().get<double>();
                }
                else if (data.key() == "co2") {
                    reply << "\n  CO2: " << data.value().get<int>();
                }
            }
            reply << "\n";
        }

        m_tdClient.sendTextMessage(
            jMessage["chat_id"].get<int64_t>(),
            jMessage["message_id"].get<int64_t>(),
            reply.str()
        );

        return;
    }
    else if (jMessage["reply"] == dome::message::command::Statistic) {
        spdlog::debug("reply processing...");

        if (!CheckJsonMessageForKeys(jMessage, { "path", "chat_id", "message_id" })) return;

        m_tdClient.sendPhoto(
            jMessage["chat_id"].get<int64_t>(),
            jMessage["message_id"].get<int64_t>(),
            jMessage["path"].get<std::string>()
        );

        return;
    }
    else if (jMessage["reply"] == dome::message::command::IpCamera) {
        spdlog::debug("reply processing...");

        if (!CheckJsonMessageForKeys(jMessage, { "quality", "path", "chat_id", "message_id" })) return;

        if (jMessage["quality"] == "sd") {
            m_tdClient.sendPhoto(
                jMessage["chat_id"].get<int64_t>(),
                jMessage["message_id"].get<int64_t>(),
                jMessage["path"].get<std::string>()
            );
        }
        else if (jMessage["quality"] == "hd") {
            m_tdClient.sendImage(
                jMessage["chat_id"].get<int64_t>(),
                jMessage["message_id"].get<int64_t>(),
                jMessage["path"].get<std::string>()
            );
        }

        return;
    }

    spdlog::debug("skip reply");
}

}
}
