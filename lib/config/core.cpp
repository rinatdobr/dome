#include "core.h"

#include "utils/utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Core::Core(const std::string &path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse core config file: {}", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
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

const std::vector<dome::config::IpCamera> &Core::ipCameras() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_ipCameras;
}

bool Core::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "database", "config_files" })) {
        spdlog::error("Invalid core JSON: \" {} \"", jConfig.dump());
        return false;
    }

    auto jDatabase = jConfig["database"];
    if (!CheckJsonMessageForKeys(jConfig, { "path" })) {
        spdlog::error("Invalid core database JSON: \" {} \"", jDatabase.dump());
        return false;
    }

    m_database.path = jDatabase["path"].get<std::string>();
    spdlog::info("Core database: path={}",
                                 m_database.path);

    auto jConfigFiles = jConfig["config_files"];
    for(const auto &jConfigFile : jConfigFiles) {
        if (!CheckJsonMessageForKeys(jConfigFile, { "path" })) {
            spdlog::error("Invalid core config file JSON: \" {} \"", jConfigFile.dump());
            continue;
        }

        auto path = jConfigFile["path"].get<std::string>();
        spdlog::info("Core config file: path={}",
                                        path);

        Provider provider(path);
        IpCamera ipCamera(path);

        m_providers.push_back(provider);
        m_ipCameras.push_back(ipCamera);
    }

    return true;
}

}
}
