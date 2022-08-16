#include "runner.h"

#include <algorithm>
#include <thread>
#include <spdlog/spdlog.h>

Runner::Runner(std::vector<std::unique_ptr<dome::config::Command>> &&configCommands)
    : m_configCommands(std::move(configCommands))
    , m_index(0)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_configCommands.size() == 0) {
        spdlog::error("No command to run");
        return;
    }

    setupSchedule();
}

void Runner::run()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int i = 0;
    while (true) {
        std::unique_ptr<dome::config::Command> &currentConfigCommand = nextConfigCommand();
        auto diff = std::chrono::seconds(currentConfigCommand->nextTimeFrameSec()) - m_lastExecutionTime;

        if (diff.count() > 0) {
            spdlog::info("Sleeping for {} seconds...", diff.count());
            std::this_thread::sleep_for(diff);
        }


        spdlog::debug("Executing command {}...", currentConfigCommand->command()->name());
        auto result = currentConfigCommand->command()->execute();
        spdlog::debug("Result={}", result.isValid());

        currentConfigCommand->io()->write(result);

        if (diff.count() > 0) {
            m_lastExecutionTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
        }

        currentConfigCommand->setNextTimeFrameSec();
    }
}

void Runner::setupSchedule()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_startTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    m_lastExecutionTime = m_startTime;
    for (auto &command : m_configCommands) {
        command->setNextTimeFrameSec(
            m_startTime.count() + command->getPeriodSec()
        );
    }
}

std::unique_ptr<dome::config::Command> &Runner::nextConfigCommand()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

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
