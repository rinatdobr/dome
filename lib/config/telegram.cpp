#include "telegram.h"
#include "utils/utils.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Telegram::Telegram(const std::string &path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse telegram config file: {}", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
}

int Telegram::appId() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_appId;
}

std::string Telegram::appHash() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_appHash;
}

std::vector<int64_t> Telegram::chatIds() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_chatIds;
}

std::string Telegram::login() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_login;
}

std::string Telegram::pass() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_pass;
}

std::string Telegram::logLevel() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_logLevel;
}

uint Telegram::refreshPeriodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_refreshPeriodSec;
}

bool Telegram::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "telegram" })) {
        spdlog::error("Invalid telegram main JSON: [{}]", jConfig.dump());
        return false;
    }

    nlohmann::json jTelegram = jConfig["telegram"];
    if (!CheckJsonMessageForKeys(jTelegram, { "app_id", "app_hash", "login", "pass", "log_level", "refresh_period", "chat_ids" })) {
        spdlog::error("Invalid telegram JSON: [{}]", jTelegram.dump());
        return false;
    }

    m_appId = jTelegram["app_id"].get<int>();
    m_appHash = jTelegram["app_hash"].get<std::string>();
    m_login = jTelegram["login"].get<std::string>();
    m_pass = jTelegram["pass"].get<std::string>();
    m_logLevel = jTelegram["log_level"].get<std::string>();
    m_refreshPeriodSec = PeriodToSeconds(jTelegram["refresh_period"].get<std::string>());
    auto jChatIds = jTelegram["chat_ids"];
    for (nlohmann::json::iterator it = jChatIds.begin(); it != jChatIds.end(); ++it) {
        m_chatIds.push_back(*it);
    }

    spdlog::info("Telegram: appId={} appHash={} chatIds.size()={} login={} pass={} logLevel={} refreshPeriodSec={}",
                            m_appId, m_appHash, m_chatIds.size(), m_login, m_pass, m_logLevel, m_refreshPeriodSec);

    return true;
}

}
}
