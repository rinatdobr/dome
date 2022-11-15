#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <utility>

uint PeriodToSeconds(const std::string &period);

std::string GetRequestTopic(const std::string &providerId);
std::string GetReplyTopic(const std::string &providerId);

std::pair<int, std::string> Exec(const std::string cmdLine);
std::string GetTmpName(const std::string id);

#endif
