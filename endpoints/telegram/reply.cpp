#include "reply.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include <config/provider.h>
#include <utils/utils.h>

namespace dome {
namespace message {

const std::string InfoRequestStr("/info");
const std::string StatisticRequestStr("/statistic");
const std::string IpCameraRequestStr("/ipcamera");

Reply::Reply(dome::data::TdClient &tdClient)
    : m_tdClient(tdClient)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Reply::~Reply()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Reply::process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!CheckJsonMessageForKeys(jMessage, { "type" })) {
        spdlog::error("{}", ErrorMessage(ErrorNum::JsonKeyNoType));
        return;
    };

    if (jMessage["type"] == InfoRequestStr) {
        spdlog::debug("reply processing...");

        if (!CheckJsonMessageForKeys(jMessage, { "data", "chat_id", "message_id" })) return;

        std::ostringstream reply;
        for (const auto &location : jMessage["data"].items()) {
            reply << location.key() << ":";
            for (const auto &data : location.value().items()) {
                if (data.key() == "temperature") {
                    reply << "\nТемпература: " << std::setprecision(3) << data.value().get<double>();
                }
                else if (data.key() == "humidity") {
                    reply << "\nВлажность: " << std::setprecision(3) << data.value().get<double>();
                }
                else if (data.key() == "co2") {
                    reply << "\nCO2: " << data.value().get<int>();
                }
            }
        }

        m_tdClient.sendTextMessage(
            jMessage["chat_id"].get<int64_t>(),
            jMessage["message_id"].get<int64_t>(),
            reply.str()
        );

        return;
    }
    else if (jMessage["type"] == StatisticRequestStr) {
        spdlog::debug("reply processing...");

        if (!CheckJsonMessageForKeys(jMessage, { "path", "chat_id", "message_id" })) return;

        m_tdClient.sendPhoto(
            jMessage["chat_id"].get<int64_t>(),
            jMessage["message_id"].get<int64_t>(),
            jMessage["path"].get<std::string>()
        );

        return;
    }
    else if (jMessage["type"] == IpCameraRequestStr) {
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
