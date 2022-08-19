#ifndef CONFIG_DB_H
#define CONFIG_DB_H

#include <string>
#include <memory>

#include <io/io.h>

#include "config.h"

namespace dome {
namespace config {

class Db : public Config
{
public:
    explicit Db(const std::string &configPath);

    std::shared_ptr<dome::io::Io> io();

private:
    std::shared_ptr<dome::io::Io> m_dbWriter;

    void parseConfig() override;
};

}
}

#endif
