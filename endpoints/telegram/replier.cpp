#include "replier.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include <config/provider.h>

namespace dome {
namespace data {

const std::string InfoRequestStr("/info");
const std::string StatisticRequestStr("/statistic");

Replier::Replier(dome::data::TdClient &tdClient)
    : m_tdClient(tdClient)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Replier::~Replier()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

void Replier::process(dome::mosq::Mosquitto &, const dome::config::Provider &provider, nlohmann::json &jMessage)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (jMessage["type"] == InfoRequestStr) {
        spdlog::debug("reply processing...");

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

        m_tdClient.sendPhoto(
            jMessage["chat_id"].get<int64_t>(),
            jMessage["message_id"].get<int64_t>(),
            jMessage["path"].get<std::string>()
        );

        return;
    }

    spdlog::debug("skip reply");
}

}
}
