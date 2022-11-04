#include "dir.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <io/io.h>
#include <io/dir.h>

namespace dome {
namespace config {

Dir::Config::Config(const std::string &name,
                   const std::string &path)
    : m_name(name)
    , m_path(path)
{
    spdlog::trace("{}:{} {} name={} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, path);
}

Dir::Dir(const std::string &configPath)
    : File(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parse();
}

const std::map<std::string, Dir::Config> Dir::dirConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_dirs;
}

void Dir::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDbs = jConfig["dirs"];
    for(const auto &jDb : jDbs) {
        if (jDb.size() != 2) {
            spdlog::error("Invalid dir format: \" {} \"", jDb.dump());
            continue;
        }

        std::string name = jDb["name"].get<std::string>();
        std::string path = jDb["path"].get<std::string>();

        spdlog::info("Dir config: name={} path={}",
            name, path);

        m_dirs.insert(std::make_pair(name, Dir::Config(name, path)));
    }
}

}
}
