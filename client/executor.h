#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string>
#include <vector>
#include <memory>

enum class CommandId
{
    Invalid,
    SystemTemperature
};

class Command
{
public:
    Command(CommandId commandId, const std::vector<std::string> &args);
    virtual ~Command();

    virtual bool execute() = 0;

protected:
    CommandId m_commandId;
    std::vector<std::string> m_args;
};

class SystemInfoCommand : public Command
{
public:
    SystemInfoCommand(CommandId commandId, const std::vector<std::string> &args);

    bool execute() override;
};

class Parser
{
public:
    static std::unique_ptr<Command> Parse(const std::string &line);

private:
    Parser();
};

class Executor
{
public:
    Executor();

    static bool ParseAndExecute(const std::string &line);
private:
};

#endif
