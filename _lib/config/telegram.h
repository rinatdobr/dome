#ifndef CONFIG_TELEGRAM_H
#define CONFIG_TELEGRAM_H

#include <string>
#include <vector>

#include "file.h"

namespace dome {
namespace config {

class Telegram : public File
{
public:
struct Config
{
    Config() {}

    Config(int appId,
           const std::string &appHash,
           const std::vector<std::string> &chatIds,
           const std::string &login,
           const std::string &pass,
           const std::string &logLevel,
           const std::string &refreshPeriod);

    int m_appId;
    std::string m_appHash;
    std::vector<int64_t> m_chatIds;
    std::string m_login;
    std::string m_pass;
    std::string m_logLevel;
    uint m_refreshPeriodSec;
};

    explicit Telegram(const std::string &configPath);

    const Config telegramConfig() const;

private:
    void parse() override;

    Config m_telegram;
};

}
}

#endif
