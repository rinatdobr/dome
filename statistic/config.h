#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>
#include <memory>

#include "command.h"

class Config
{
public:
    explicit Config(const std::string &path);

    std::vector<std::unique_ptr<Command>> &&commands();

private:
    std::string m_path;
    std::vector<std::unique_ptr<Command>> m_commands;

    void parse(const std::string &configData);
};

#endif
