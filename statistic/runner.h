#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <chrono>
#include <map>

#include "command.h"

class Runner
{
public:
    Runner(
        std::vector<std::unique_ptr<Command>> &&commands
    );

    void run();

private:
    std::vector<std::unique_ptr<Command>> m_commands;
    std::chrono::seconds m_startTime;
    std::chrono::seconds m_lastExecutionTime;
    uint m_index;
    // std::chrono::time_point<std::chrono::system_clock> m_startTime;

    void setupSchedule();
    std::unique_ptr<Command> &nextCommand();
};

#endif
