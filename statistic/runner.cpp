#include "runner.h"

#include <algorithm>
#include <thread>
#include <spdlog/spdlog.h>

Runner::Runner(const std::map<std::string, dome::config::Db::Config> &dbConfig,
               const std::vector<dome::config::Statistic::Config> &statisticConfig)
    : m_dbs(dome::io::Db::Create(dbConfig))
    , m_commands(Command::Create(statisticConfig))
    , m_index(0)
{
    spdlog::trace("{}:{} {} statisticConfig.size()={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, statisticConfig.size());

    for (auto &command : m_commands) {
        if (command->ioType() == Command::IoType::Db) {
            command->setIo(m_dbs[command->ioName()]);
        }
    }

    setupSchedule();
}

void Runner::run()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    int i = 0;
    while (true) {
        std::unique_ptr<Command> &currentCommand = nextCommand();
        auto diff = std::chrono::seconds(currentCommand->nextTimeFrameSec()) - m_lastExecutionTime;

        if (diff.count() > 0) {
            spdlog::info("Sleeping for {} seconds...", diff.count());
            std::this_thread::sleep_for(diff);
        }

        spdlog::debug("Executing command {}...", currentCommand->command()->name());
        auto result = currentCommand->command()->execute();
        spdlog::debug("Result={}", result.isValid());

        currentCommand->io()->write(result);

        if (diff.count() > 0) {
            m_lastExecutionTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
        }

        currentCommand->setNextTimeFrameSec();
    }
}

void Runner::setupSchedule()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    m_startTime = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch();
    m_lastExecutionTime = m_startTime;
    for (auto &command : m_commands) {
        command->setNextTimeFrameSec(
            m_startTime.count() + command->getPeriodSec()
        );
    }
}

std::unique_ptr<Command> &Runner::nextCommand()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto minIt = std::min_element(
        m_commands.begin(),
        m_commands.end(),
        [this](const std::unique_ptr<Command> &c1, const std::unique_ptr<Command> &c2) {
        return
            (std::chrono::seconds(c1->nextTimeFrameSec()) - m_lastExecutionTime)
            <
            (std::chrono::seconds(c2->nextTimeFrameSec()) - m_lastExecutionTime);
    });

    return *minIt;
}
