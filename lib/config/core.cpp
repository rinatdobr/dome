#include "core.h"

#include "utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Core::Core(std::string path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
        path);

    parse();
}

void Core::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDatabase = jConfig["database"];
    if (jDatabase.size() != 1) {
        spdlog::error("Invalid database format: \" {} \"", jDatabase.dump());
        return;
    }
    m_database.path = jDatabase["path"].get<std::string>();

    auto jConfigFiles = jConfig["config_files"];
    for(const auto &jConfigFile : jConfigFiles) {
        if (jConfigFile.size() != 1) {
            spdlog::error("Invalid source format: \" {} \"", jConfigFile.dump());
            continue;
        }

        Provider provider(jConfigFile["path"].get<std::string>());

        m_providers.push_back(provider);
    }


}

Database Core::database() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_database;
}

const std::vector<dome::config::Provider> &Core::providers() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_providers;
}

}
}
