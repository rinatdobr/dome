#ifndef CONFIG_H
#define CONFIG_H

#include "command.h"

#include <string>
#include <vector>

class Config
{
public:
    explicit Config(const std::string &path);

    std::vector<Command> commands() const;

private:
    std::string m_path;
    std::vector<Command> m_commands;

    void parse(const std::string &configData);
};

#endif
