#include "runner.h"

#include <iostream>
#include <algorithm>
#include <thread>

Runner::Runner(
    std::vector<std::unique_ptr<dome::config::Command>> &&configCommands
)
    : m_configCommands(std::move(configCommands))
    , m_index(0)
{
    if (m_configCommands.size() == 0) {
        std::cerr << "No command to schedule" << std::endl;
        return;
    }

    setupSchedule();
}

void Runner::run()
{
    int i = 0;
    while (true) {
        std::unique_ptr<dome::config::Command> &currentConfigCommand = nextConfigCommand();
        std::cout << "==" << std::endl;
        auto diff = std::chrono::seconds(currentConfigCommand->nextTimeFrameSec()) - m_lastExecutionTime;

        if (diff.count() > 0) {
            std::cout << "Sleeping for " << diff.count() << " seconds..." << std::endl;
            std::this_thread::sleep_for(diff);
        }

        std::cout << "Executing command " << currentConfigCommand->command()->name() << " ..." << std::endl;

        auto result = currentConfigCommand->command()->execute();

        std::cout << "result " << result.toString() << std::endl;

        currentConfigCommand->io()->write(result);

        if (diff.count() > 0) {
            m_lastExecutionTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
        }

        currentConfigCommand->setNextTimeFrameSec();
    }
}

void Runner::setupSchedule()
{
    m_startTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    m_lastExecutionTime = m_startTime;
    std::cout << m_startTime.count() << std::endl;
    for (auto &command : m_configCommands) {
        command->setNextTimeFrameSec(
            m_startTime.count() + command->getPeriodSec()
        );
        std::cout << command->command()->name() << " " << command->getPeriodSec() << " " << command->nextTimeFrameSec() << std::endl;
    }
}

std::unique_ptr<dome::config::Command> &Runner::nextConfigCommand()
{
    auto minIt = std::min_element(
        m_configCommands.begin(),
        m_configCommands.end(),
        [this](const std::unique_ptr<dome::config::Command> &c1, const std::unique_ptr<dome::config::Command> &c2) {
        return
            (std::chrono::seconds(c1->nextTimeFrameSec()) - m_lastExecutionTime)
            <
            (std::chrono::seconds(c2->nextTimeFrameSec()) - m_lastExecutionTime);
    });

    return *minIt;
}
