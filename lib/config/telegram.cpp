#include "telegram.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <commands/parser.h>
#include <utils.h>

namespace dome {
namespace config {

Telegram::Config::Config(const int appId,
                         const std::string &appHash,
                         const std::vector<std::string> &chatIds,
                         const std::string &login,
                         const std::string &pass,
                         const std::string &logLevel,
                         const std::string &refreshPeriod)
    : m_appId(appId)
    , m_appHash(appHash)
    , m_login(login)
    , m_pass(pass)
    , m_logLevel(logLevel)
{
    spdlog::trace("{}:{} {} appId={} appHash={} chatIds.size()={} login={} pass={} logLevel={} refreshPeriod={}",
        __FILE__, __LINE__, __PRETTY_FUNCTION__, appId, appHash, chatIds.size(), login, pass, logLevel, refreshPeriod);

    m_refreshPeriodSec = PeriodToSeconds(refreshPeriod);

    for (const auto &chatId : chatIds) {
        m_chatIds.push_back(std::strtoll(chatId.c_str(), nullptr, 10));
    }
}

Telegram::Telegram(const std::string &configPath)
    : File(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parse();
}

const Telegram::Config Telegram::telegramConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_telegram;
}

void Telegram::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    int appId = jConfig["app_id"].get<int>();
    std::string appHash = jConfig["app_hash"].get<std::string>();
    std::string login = jConfig["login"].get<std::string>();
    std::string pass = jConfig["pass"].get<std::string>();
    std::string logLevel = jConfig["log_level"].get<std::string>();
    std::string refreshPeriod = jConfig["refresh_period"].get<std::string>();
    auto jChatIds = jConfig["chat_ids"];
    std::vector<std::string> chatIds;
    for (nlohmann::json::iterator it = jChatIds.begin(); it != jChatIds.end(); ++it) {
        chatIds.push_back(*it);
    }

    spdlog::info("Telegram config: appId={} appHash={} chatIds.size()={} login={} pass={} logLevel={} refreshPeriod={}", appId, appHash, chatIds.size(), login, pass, logLevel, refreshPeriod);

    m_telegram = Config(appId, appHash, chatIds, login, pass, logLevel, refreshPeriod);
}

}
}
