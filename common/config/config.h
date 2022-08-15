#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <string>
#include <vector>
#include <memory>

namespace dome {
namespace config {

class Config
{
public:
    explicit Config(const std::string &configPath);

protected:
    std::string readConfigFile();
    virtual void parseConfig() = 0;

private:
    std::string m_configPath;
};

}
}

#endif
