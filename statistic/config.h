#ifndef CONFIG_H
#define CONFIG_H

#include <config/commands.h>
#include <config/db.h>

class Config : public dome::config::Db, public dome::config::Commands
{
public:
    Config(const std::string &dbConfigPath, const std::string &commandsConfigPath);
};

#endif
