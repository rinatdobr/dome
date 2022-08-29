#ifndef CONFIG_STATISTIC_H
#define CONFIG_STATISTIC_H

#include <string>
#include <vector>
#include <memory>

#include "file.h"

namespace dome {
namespace config {

class Statistic : public File
{
public:
struct Config
{
    Config(const std::string &name,
           const std::string &period,
           const std::vector<std::string> &args,
           const std::string &outputType,
           const std::string &outputName);

    const std::string m_name;
    const std::string m_period;
    const std::vector<std::string> m_args;
    const std::string m_outputType;
    const std::string m_outputName;
};

    explicit Statistic(const std::string &configPath);

    const std::vector<Config> statisticConfig() const;

private:
    void parse() override;

    std::vector<Config> m_statistic;
};

}
}

#endif
