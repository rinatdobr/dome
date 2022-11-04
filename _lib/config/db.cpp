#include "db.h"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <io/io.h>
#include <io/db.h>

namespace dome {
namespace config {

Db::Config::Config(const std::string &name,
                   const std::string &path)
    : m_name(name)
    , m_path(path)
{
    spdlog::trace("{}:{} {} name={} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, name, path);
}

Db::Db(const std::string &configPath)
    : File(configPath)
{
    spdlog::trace("{}:{} {} configPath={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, configPath);

    parse();
}

const std::map<std::string, Db::Config> Db::dbConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_dbs;
}

void Db::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);

    auto jDbs = jConfig["databases"];
    for(const auto &jDb : jDbs) {
        if (jDb.size() != 2) {
            spdlog::error("Invalid database format: \" {} \"", jDb.dump());
            continue;
        }

        std::string name = jDb["name"].get<std::string>();
        std::string path = jDb["path"].get<std::string>();

        spdlog::info("Database config: name={} path={}",
            name, path);

        m_dbs.insert(std::make_pair(name, Db::Config(name, path)));
    }
}

}
}
