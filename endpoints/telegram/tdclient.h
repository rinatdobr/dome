#ifndef ENDPOINTS_TD_CLIENT_H
#define ENDPOINTS_TD_CLIENT_H

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <queue>

#include <config/provider.h>
#include <data/provider.h>
#include <config/telegram.h>
#include <mosquitto/sender.h>

namespace dome {
namespace data {

class TdClient : public dome::data::Provider {
public:

    TdClient(const dome::config::Telegram &telegramConfig, const dome::config::Provider &providerConfig, dome::mosq::Sender::Trigger &senderTrigger);

    void run();
    void sendTextMessage(int64_t chatId, int64_t messageId, const std::string text);
    void sendPhoto(int64_t chatId, int64_t messageId, const std::string path);

protected:
    bool prepareData() override;
    nlohmann::json getData() override;

private:
    const dome::config::Provider &m_providerConfig;
    dome::mosq::Sender::Trigger &m_senderTrigger;
    std::queue<std::string> m_requests;

    uint m_refreshPeriodSec;
    std::string m_login;
    std::string m_pass;
    td::td_api::int32 m_appId;
    std::string m_appHash;
    std::vector<int64_t> m_chatIds;

    std::unique_ptr<td::ClientManager> m_clientManager;
    td::ClientManager::ClientId m_clientId;

    bool m_isAuthenticated;

    using Object = td::td_api::object_ptr<td::td_api::Object>;

    td::td_api::object_ptr<td::td_api::AuthorizationState> m_authorizationState;
    td::ClientManager::RequestId m_currentQueryId;
    td::ClientManager::RequestId m_authenticationQueryId;

    std::map<td::ClientManager::RequestId, std::function<void(Object)>> m_handlers;
    std::map<std::int64_t, td::td_api::object_ptr<td::td_api::user>> m_users;
    std::map<std::int64_t, std::string> m_chatTitle;

    void sendQuery(td::td_api::object_ptr<td::td_api::Function> f, std::function<void(Object)> handler);
    void processResponse(td::ClientManager::Response response);
    std::string getUserName(std::int64_t userId) const;
    std::string getChatTitle(std::int64_t chatId) const;
    void processUpdate(td::td_api::object_ptr<td::td_api::Object> update);
    auto createAuthenticationQueryHandler();
    void onAuthorizationStateUpdate();
    void checkAuthenticationError(Object object);
    td::ClientManager::RequestId nextQueryId();
};

}
}

#endif
