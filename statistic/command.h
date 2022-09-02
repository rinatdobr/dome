#ifndef CONFIG_COMMAND_H
#define CONFIG_COMMAND_H

#include <string>
#include <memory>

#include <config/statistic.h>
#include <commands/command.h>
#include <io/io.h>

class Command
{
public:

enum class IoType
{
    Invalid,
    Db,
    Dir
};
    static std::vector<std::unique_ptr<Command>> Create(const std::vector<dome::config::Statistic::Config> &statisticConfig);

    Command(std::unique_ptr<command::Command> &&command,
            uint period,
            const std::string &ioType,
            const std::string &ioName);

    std::string ioName() const;
    IoType ioType() const;

    uint getPeriodSec() const;
    void setPeriodSec(uint periodSec);

    uint nextTimeFrameSec() const;
    void setNextTimeFrameSec(uint nextTimeFrameSec = 0);

    std::shared_ptr<dome::io::Io> io();
    void setIo(std::shared_ptr<dome::io::Io> io);

    const std::unique_ptr<command::Command> &command() const;

private:
    std::unique_ptr<command::Command> m_command;
    std::shared_ptr<dome::io::Io> m_io;
    uint m_periodSec;
    uint m_nextTimeFrameSec;
    std::string m_ioName;
    IoType m_ioType;
};

#endif
