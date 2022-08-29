#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <chrono>
#include <map>

#include <config/statistic.h>
#include <io/db.h>
#include "command.h"

class Runner
{
public:
    Runner(const std::map<std::string, dome::config::Db::Config> &dbConfig,
           const std::vector<dome::config::Statistic::Config> &statisticConfig);

    void run();

private:
    std::map<std::string, std::shared_ptr<dome::io::Db>> m_dbs;
    std::vector<std::unique_ptr<Command>> m_commands;
    std::chrono::seconds m_startTime;
    std::chrono::seconds m_lastExecutionTime;
    uint m_index;

    void setupSchedule();
    std::unique_ptr<Command> &nextCommand();
};

#endif
