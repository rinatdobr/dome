#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <chrono>
#include <map>

#include <config/command.h>

class Runner
{
public:
    Runner(
        std::vector<std::unique_ptr<dome::config::Command>> &&configCommands
    );

    void run();

private:
    std::vector<std::unique_ptr<dome::config::Command>> m_configCommands;
    std::chrono::seconds m_startTime;
    std::chrono::seconds m_lastExecutionTime;
    uint m_index;

    void setupSchedule();
    std::unique_ptr<dome::config::Command> &nextConfigCommand();
};

#endif
