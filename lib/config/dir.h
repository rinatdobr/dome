#ifndef CONFIG_DIR_H
#define CONFIG_DIR_H

#include <string>
#include <map>

#include <io/io.h>

#include "file.h"

namespace dome {
namespace config {

class Dir : public File
{
public:
struct Config
{
    Config(const std::string &name, const std::string &path);

    std::string m_name;
    std::string m_path;
};

    explicit Dir(const std::string &configPath);

    const std::map<std::string, Config> dirConfig() const;

private:
    void parse() override;

    std::map<std::string, Config> m_dirs;
};

}
}

#endif
