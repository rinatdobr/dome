#ifndef CONFIG_COMMAND_H
#define CONFIG_COMMAND_H

#include <string>
#include <memory>

#include <commands/command.h>
#include <io/io.h>

namespace dome {
namespace config {

class Command
{
public:

enum class IoType
{
    Invalid,
    Db,
    File
};

    Command(
        std::unique_ptr<command::Command> &&command,
        uint period,
        const std::string &ioType
    );

    IoType ioType();

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
    IoType m_ioType;
};

}
}

#endif
