#include "db.h"

#include <iostream>
#include <nlohmann/json.hpp>

#include <io/io.h>
#include <io/db.h>

namespace dome {
namespace config {

Db::Db(const std::string &configPath)
    : Config(configPath)
{
    parseConfig();
}

std::shared_ptr<dome::io::Io> Db::io()
{
    return m_dbWriter;
}

void Db::parseConfig()
{
    auto configData = readConfigFile();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDatabase = jConfig["database"];
    std::cout << "jDatabase " << jDatabase << std::endl;
    std::string dbName =  jDatabase["name"].get<std::string>();
    std::cout << "dbName " << dbName << std::endl;
    std::string dbEnv = std::getenv(jDatabase["env"].get<std::string>().c_str());
    std::cout << "dbEnv " << dbEnv << std::endl;
    std::string fullDbPath = dbEnv + "/" + dbName;
    m_dbWriter = std::make_shared<dome::io::Db>(fullDbPath);
}

}
}
