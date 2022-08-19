#ifndef CONFIG_COMMANDS_H
#define CONFIG_COMMANDS_H

#include <string>
#include <vector>
#include <memory>

#include "config.h"
#include "command.h"

namespace dome {
namespace config {

class Commands : public Config
{
public:
    explicit Commands(const std::string &configPath);

    std::vector<std::unique_ptr<Command>> &commands();

private:
    std::vector<std::unique_ptr<Command>> m_commands;

    void parseConfig() override;
};

}
}

#endif
