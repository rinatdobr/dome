#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <chrono>
#include <map>

#include "command.h"

class Runner
{
public:
    Runner(const std::vector<Command> &commands);

    void run();

private:
    std::vector<Command> m_commands;
    std::chrono::seconds m_startTime;
    std::chrono::seconds m_lastExecutionTime;
    uint m_index;
    // std::chrono::time_point<std::chrono::system_clock> m_startTime;

    void setupSchedule();
    Command &nextCommand();
};

#endif
