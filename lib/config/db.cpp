#include "db.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <io/io.h>
#include <io/db.h>

namespace dome {
namespace config {

Db::Db(const std::string &configPath)
    : Config(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parseConfig();
}

std::shared_ptr<dome::io::Io> Db::io()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_dbWriter;
}

void Db::parseConfig()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = readConfigFile();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDatabase = jConfig["database"];
    std::string dbName =  jDatabase["name"].get<std::string>();
    std::string dbEnv = std::getenv(jDatabase["env"].get<std::string>().c_str());
    std::string fullDbPath = dbEnv + "/" + dbName;

    spdlog::info("Database IO: name={}, env={}, fullPath={}", dbName, dbEnv, fullDbPath);

    m_dbWriter = std::make_shared<dome::io::Db>(fullDbPath);
}

}
}
