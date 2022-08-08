#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command
{
public:
    Command(const std::string &command);

    const std::string &getCommand() const;

    uint getPeriodSec() const;
    void setPeriodSec(uint periodSec);

    uint nextTimeFrameSec() const;
    void setNextTimeFrameSec(uint nextTimeFrameSec = 0);

    void execute();

private:
    std::string m_command;
    uint m_periodSec;
    uint m_nextTimeFrameSec;
};

#endif
