#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>

uint PeriodToSeconds(const std::string &period);

std::string GetRequestTopic(const std::string &providerId);
std::string GetReplyTopic(const std::string &providerId);

std::pair<int, std::string> Exec(const std::string cmdLine);
std::string GetTmpName(const std::string id);

bool CheckJsonMessageForKeys(const nlohmann::json &jMessage, const std::vector<std::string> &keys);

enum class ErrorNum
{
    JsonKeyNoType
};

std::string ErrorMessage(ErrorNum errorNum);

#endif
