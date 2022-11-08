#ifndef UTILS_H
#define UTILS_H

#include <string>

uint PeriodToSeconds(const std::string &period);

std::string GetRequestTopic(const std::string &providerId);
std::string GetReplyTopic(const std::string &providerId);

#endif
