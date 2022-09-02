#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <spdlog/spdlog.h>

uint PeriodToSeconds(const std::string &period);
std::vector<std::string> ParseArgs(const std::string &args);

#endif
