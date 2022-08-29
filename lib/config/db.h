#ifndef CONFIG_DB_H
#define CONFIG_DB_H

#include <string>
#include <map>

#include <io/io.h>

#include "file.h"

namespace dome {
namespace config {

class Db : public File
{
public:
struct Config
{
    Config(const std::string &name, const std::string &path);

    std::string m_name;
    std::string m_path;
};

    explicit Db(const std::string &configPath);

    const std::map<std::string, Config> dbConfig() const;

private:
    void parse() override;

    std::map<std::string, Config> m_dbs;
};

}
}

#endif
