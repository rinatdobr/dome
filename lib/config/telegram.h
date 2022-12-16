#ifndef CONFIG_TELEGRAM_H
#define CONFIG_TELEGRAM_H

#include <string>
#include <vector>

#include "utils/file.h"

namespace dome {
namespace config {

class Telegram : public dome::utils::File
{
public:
    explicit Telegram(const std::string &path);

    int appId() const;
    std::string appHash() const;
    std::vector<int64_t> chatIds() const;
    std::string login() const;
    std::string pass() const;
    std::string logLevel() const;
    uint refreshPeriodSec() const;

private:
    bool parse();

    int m_appId;
    std::string m_appHash;
    std::vector<int64_t> m_chatIds;
    std::string m_login;
    std::string m_pass;
    std::string m_logLevel;
    uint m_refreshPeriodSec;
};

}
}

#endif
