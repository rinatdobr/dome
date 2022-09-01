#include "tdclient.h"

#include "commandor.h"

#include <chrono>
#include <thread>

// overloaded
namespace detail {
template <class... Fs>
struct overload;

template <class F>
struct overload<F> : public F {
  explicit overload(F f) : F(f) {
  }
};
template <class F, class... Fs>
struct overload<F, Fs...>
    : public overload<F>
    , public overload<Fs...> {
  overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {
  }
  using overload<F>::operator();
  using overload<Fs...>::operator();
};
}  // namespace detail

template <class... F>
auto overloaded(F... f) {
  return detail::overload<F...>(f...);
}

TdClient::TdClient(const dome::config::Telegram::Config &telegramConfig)
    : m_clientId(0)
    , m_refreshSec(telegramConfig.m_refreshPeriodSec)
    , m_login(telegramConfig.m_login)
    , m_pass(telegramConfig.m_pass)
    , m_appId(telegramConfig.m_appId)
    , m_appHash(telegramConfig.m_appHash)
    , m_chatIds(telegramConfig.m_chatIds)
	, m_isAuthenticated(false)
    , m_currentQueryId(0)
    , m_authenticationQueryId(0)
{
    td::ClientManager::execute(td::td_api::make_object<td::td_api::setLogVerbosityLevel>(2));

    m_clientManager = std::make_unique<td::ClientManager>();
    m_clientId = m_clientManager->create_client_id();

    sendQuery(td::td_api::make_object<td::td_api::getOption>("version"), {});
}

void TdClient::run() {
    bool updating = false;

	while (true) {
		if (!m_isAuthenticated) {
			processResponse(m_clientManager->receive(10));
		} else {
			std::cout << "Enter action [q] quit [u] check for updates and request results [c] show chats [m <chatId> "
				"<text>] send message [me] show self [l] logout: "
				<< std::endl;
			std::string action;
            updating = true;
            if (!updating) {
                std::string line;
                std::getline(std::cin, line);
                std::istringstream ss(line);
                if (!(ss >> action)) {
                    continue;
                }
            }
            else {
                action = "u";
				std::cout << "Sleeping..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(m_refreshSec));
				std::cout << "Sleeping... Done." << std::endl;
            }
			if (action == "q") {
				return;
			}
			if (action == "u") {
				std::cout << "Checking for updates..." << std::endl;
				while (true) {
					auto response = m_clientManager->receive(0);
					if (response.object) {
						processResponse(std::move(response));
					} else {
						break;
					}
				}
                updating = true;
			} else if (action == "close") {
				std::cout << "Closing..." << std::endl;
				sendQuery(td::td_api::make_object<td::td_api::close>(), {});
			} else if (action == "me") {
				sendQuery(td::td_api::make_object<td::td_api::getMe>(),
				[this](Object object) { std::cout << to_string(object) << std::endl; });
			} else if (action == "l") {
				std::cout << "Logging out..." << std::endl;
				sendQuery(td::td_api::make_object<td::td_api::logOut>(), {});
			} else if (action == "m") {
				std::int64_t chatId;
				// ss >> chatId;
				// ss.get();
				std::string text;
				// std::getline(ss, text);

				std::cout << "Sending message to chat " << chatId << "..." << std::endl;
				auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
				send_message->chat_id_ = chatId;
				auto message_content = td::td_api::make_object<td::td_api::inputMessageText>();
				message_content->text_ = td::td_api::make_object<td::td_api::formattedText>();
				message_content->text_->text_ = std::move(text);
				send_message->input_message_content_ = std::move(message_content);

				sendQuery(std::move(send_message), {});
			} else if (action == "c") {
				std::cout << "Loading chat list..." << std::endl;
				sendQuery(td::td_api::make_object<td::td_api::getChats>(nullptr, 20), [this](Object object) {
                    std::cout << "chat callback" << std::endl;
                    if (object->get_id() == td::td_api::error::ID) {
                        std::cout << "error" << std::endl;
                        return;
                    }
                    auto chats = td::move_tl_object_as<td::td_api::chats>(object);
                    for (auto chatId : chats->chat_ids_) {
                        std::cout << "[chatId:" << chatId << "] [title:" << m_chatTitle[chatId] << "]" << std::endl;
                    }
				});
			}
		}
	}
}

void TdClient::sendQuery(td::td_api::object_ptr<td::td_api::Function> f, std::function<void(Object)> handler) {
    auto queryId = nextQueryId();
    std::cout << "queryId: " << queryId << std::endl;
    if (handler) {
        m_handlers.emplace(queryId, std::move(handler));
    }
    m_clientManager->send(m_clientId, queryId, std::move(f));
}

void TdClient::processResponse(td::ClientManager::Response response) {
    if (!response.object) {
        return;
    }
    // std::cout << response.request_id << " " << to_string(response.object) << std::endl;
    if (response.request_id == 0) {
        return processUpdate(std::move(response.object));
    }
    auto it = m_handlers.find(response.request_id);
    if (it != m_handlers.end()) {
        it->second(std::move(response.object));
        m_handlers.erase(it);
    }
}

std::string TdClient::getUserName(std::int64_t userId) const {
    auto it = m_users.find(userId);
    if (it == m_users.end()) {
        return "unknown user";
    }
    return it->second->first_name_ + " " + it->second->last_name_;
}

std::string TdClient::getChatTitle(std::int64_t chatId) const {
    auto it = m_chatTitle.find(chatId);
    if (it == m_chatTitle.end()) {
        return "unknown chat";
    }
    return it->second;
}

void TdClient::processUpdate(td::td_api::object_ptr<td::td_api::Object> update) {
    td::td_api::downcast_call(*update,
        overloaded(
            [this](td::td_api::updateAuthorizationState &update_authorization_state) {
                m_authorizationState = std::move(update_authorization_state.authorization_state_);
                onAuthorizationStateUpdate();
            },
            [this](td::td_api::updateNewChat &update_new_chat) {
                m_chatTitle[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
            },
            [this](td::td_api::updateChatTitle &update_chat_title) {
                m_chatTitle[update_chat_title.chat_id_] = update_chat_title.title_;
            },
            [this](td::td_api::updateUser &update_user) {
                auto userId = update_user.user_->id_;
                m_users[userId] = std::move(update_user.user_);
            },
            [this](td::td_api::updateNewMessage &update_new_message) {
                auto chatId = update_new_message.message_->chat_id_;
                std::string senderName;
                td::td_api::downcast_call(*update_new_message.message_->sender_id_,
                                        overloaded(
                                            [this, &senderName](td::td_api::messageSenderUser &user) {
                                                senderName = getUserName(user.user_id_);
                                            },
                                            [this, &senderName](td::td_api::messageSenderChat &chat) {
                                                senderName = getChatTitle(chat.chat_id_);
                                            }));
                std::string text;
                if (update_new_message.message_->content_->get_id() == td::td_api::messageText::ID) {
                    text = static_cast<td::td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
                }
                std::cout << "Got message: [chatId:" << chatId << "] [from:" << senderName << "] [" << text
                          << "]" << std::endl;

                bool isFound = false;
                for (const auto &id : m_chatIds) {
                    if (chatId == id) {
                        isFound = true;
                        break;
                    }
                }

                if (isFound) {
                    std::cout << "DOME" << std::endl;
                    command::Result result = Commandor::Run(text);
                    if (result.isValid()) {
                        std::cout << "Sending message to chat " << chatId << "..." << std::endl;
                        auto send_message = td::td_api::make_object<td::td_api::sendMessage>();
                        send_message->chat_id_ = chatId;
                        switch (result.type()) {
                            case command::Result::Type::String: {
                                auto message_content = td::td_api::make_object<td::td_api::inputMessageText>();
                                message_content->text_ = td::td_api::make_object<td::td_api::formattedText>();
                                message_content->text_->text_ = std::move(result.toString());
                                send_message->input_message_content_ = std::move(message_content);
                            }
                            break;
                            case command::Result::Type::Photo: {
                                auto message_content = td::td_api::make_object<td::td_api::inputMessagePhoto>();
                                message_content->photo_ = td::td_api::make_object<td::td_api::inputFileLocal>(result.toString());
                                message_content->thumbnail_ = nullptr;
                                message_content->width_ = 1920;
                                message_content->height_ = 1080;
                                message_content->caption_ = td::td_api::make_object<td::td_api::formattedText>();
                                message_content->caption_->text_ = std::move(result.toString());
                                send_message->input_message_content_ = std::move(message_content);
                            }
                            break;
                            case command::Result::Type::Document: {
                                auto message_content = td::td_api::make_object<td::td_api::inputMessageDocument>();
                                message_content->document_ = td::td_api::make_object<td::td_api::inputFileLocal>(result.toString());
                                message_content->thumbnail_ = nullptr;
                                message_content->caption_ = td::td_api::make_object<td::td_api::formattedText>();
                                message_content->caption_->text_ = std::move(result.toString());
                                send_message->input_message_content_ = std::move(message_content);
                            }
                            break;
                        }

                        sendQuery(std::move(send_message), {});
                    }
                    else {
                        std::cout << "Result is invalid" << std::endl;
                    }
                }
            },
            [](auto &update) {

            }
        )
    );
}

auto TdClient::createAuthenticationQueryHandler() {
    return [this, id = m_authenticationQueryId](Object object) {
        if (id == m_authenticationQueryId) {
            checkAuthenticationError(std::move(object));
        }
    };
}

void TdClient::onAuthorizationStateUpdate() {
    m_authenticationQueryId++;
    td::td_api::downcast_call(
        *m_authorizationState,
        overloaded(
            [this](td::td_api::authorizationStateReady &) {
				m_isAuthenticated = true;
				std::cout << "Got authorization" << std::endl;
            },
            [this](td::td_api::authorizationStateLoggingOut &) {
				m_isAuthenticated = false;
				std::cout << "Logging out" << std::endl;
            },
            [this](td::td_api::authorizationStateClosing &) {
				std::cout << "Closing" << std::endl;
			},
            [this](td::td_api::authorizationStateClosed &) {
              	m_isAuthenticated = false;
              	std::cout << "Terminated" << std::endl;
            },
            [this](td::td_api::authorizationStateWaitCode &) {
				std::cout << "Enter authentication code: " << std::flush;
				std::string code;
				std::cin >> code;
				sendQuery(td::td_api::make_object<td::td_api::checkAuthenticationCode>(code), createAuthenticationQueryHandler());
            },
            [this](td::td_api::authorizationStateWaitPassword &) {
				std::cout << "Enter authentication password: " << std::flush;
				std::string password;
				std::getline(std::cin, password);
				sendQuery(td::td_api::make_object<td::td_api::checkAuthenticationPassword>(password), createAuthenticationQueryHandler());
            },
            [this](td::td_api::authorizationStateWaitPhoneNumber &) {
				std::cout << "Enter phone number: " << std::flush;
				std::string phone_number;
				std::cin >> phone_number;
				sendQuery(td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phone_number, nullptr), createAuthenticationQueryHandler());
            },
            [this](td::td_api::authorizationStateWaitEncryptionKey &) {
				std::cout << "Enter encryption key or DESTROY: " << std::flush;
				std::string key;
				std::getline(std::cin, key);
				if (key == "DESTROY") {
                	sendQuery(td::td_api::make_object<td::td_api::destroy>(), createAuthenticationQueryHandler());
              	} else {
                	sendQuery(td::td_api::make_object<td::td_api::checkDatabaseEncryptionKey>(std::move(key)), createAuthenticationQueryHandler());
              	}
            },
            [this](td::td_api::authorizationStateWaitTdlibParameters &) {
				std::cout << "Sent tdlib parameters" << std::endl;
				auto parameters = td::td_api::make_object<td::td_api::tdlibParameters>();
				parameters->database_directory_ = "tdlib";
				parameters->use_message_database_ = true;
				parameters->use_secret_chats_ = true;
				parameters->api_id_ = m_appId;
				parameters->api_hash_ = m_appHash;
				parameters->system_language_code_ = "en";
				parameters->device_model_ = "Desktop";
				parameters->application_version_ = "1.0";
				parameters->enable_storage_optimizer_ = true;
				sendQuery(td::td_api::make_object<td::td_api::setTdlibParameters>(std::move(parameters)), createAuthenticationQueryHandler());
            },
            [](auto &) {
                
            }
        )
    );
}

void TdClient::checkAuthenticationError(Object object) {
    if (object->get_id() == td::td_api::error::ID) {
        auto error = td::move_tl_object_as<td::td_api::error>(object);
        std::cout << "Error: " << to_string(error) << std::flush;
        onAuthorizationStateUpdate();
    }
}

td::ClientManager::RequestId TdClient::nextQueryId() {
    return ++m_currentQueryId;
}
