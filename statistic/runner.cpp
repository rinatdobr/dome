#include "runner.h"

#include <iostream>
#include <algorithm>
#include <thread>

Runner::Runner(const std::vector<Command> &commands)
    : m_commands(commands)
    , m_index(0)
{
    if (commands.size() == 0) {
        std::cerr << "No command to schedule" << std::endl;
        return;
    }

    setupSchedule();
}

void Runner::run()
{
    int i = 0;
    while (true) {
        Command &currentCommand = nextCommand();
        std::cout << "==" << std::endl;
        auto diff = std::chrono::seconds(currentCommand.nextTimeFrameSec()) - m_lastExecutionTime;

        if (diff.count() > 0) {
            std::cout << "Sleeping for " << diff.count() << " seconds..." << std::endl;
            std::this_thread::sleep_for(diff);
        }

        std::cout << "Executing command " << currentCommand.getCommand() << " ..." << std::endl;

        if (diff.count() > 0) {
            m_lastExecutionTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
        }

        currentCommand.setNextTimeFrameSec();
    }
}

void Runner::setupSchedule()
{
    m_startTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    m_lastExecutionTime = m_startTime;
    std::cout << m_startTime.count() << std::endl;
    for (auto &command : m_commands) {
        command.setNextTimeFrameSec(
            m_startTime.count() + command.getPeriodSec()
        );
        std::cout << command.getCommand() << " " << command.getPeriodSec() << " " << command.nextTimeFrameSec() << std::endl;
    }
}

Command &Runner::nextCommand()
{
    auto minIt = std::min_element(m_commands.begin(), m_commands.end(), [this](const Command &c1, const Command &c2) {
        return
            (std::chrono::seconds(c1.nextTimeFrameSec()) - m_lastExecutionTime)
            <
            (std::chrono::seconds(c2.nextTimeFrameSec()) - m_lastExecutionTime);
    });

    return *minIt;
}
