#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#include <vector>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

uint PeriodToSeconds(const std::string &period);

std::string GetRequestTopic(const std::string &endPointId);
std::string GetReplyTopic(const std::string &endPointId);

std::pair<int, std::string> Exec(const std::string cmdLine);
std::string GetTmpName(const std::string id);

std::string PingMessage();

bool CheckJsonMessageForKeys(const nlohmann::json &jMessage, const std::vector<std::string> &keys);

#endif
