#ifndef UTILS_H
#define UTILS_H

#include <string>

uint PeriodToSeconds(const std::string &period);

std::string GetCommandTopic(const std::string &providerId);
std::string GetReplyTopic(const std::string &providerId);

#endif
