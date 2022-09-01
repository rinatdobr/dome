#ifndef CONFIG_H
#define CONFIG_H

#include <config/telegram.h>

class Config : public dome::config::Telegram
{
public:
    Config();
};

#endif
