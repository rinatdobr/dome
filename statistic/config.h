#ifndef CONFIG_H
#define CONFIG_H

#include <config/db.h>
#include <config/dir.h>
#include <config/statistic.h>

class Config
    : public dome::config::Db
    , public dome::config::Dir
    , public dome::config::Statistic
{
public:
    Config();
};

#endif
