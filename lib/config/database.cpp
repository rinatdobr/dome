#include "database.h"

#include "utils/utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Database::Database(const std::string &path)
    : File(path)
{
    spdlog::trace("{}:{} {} path=\"{}\"", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse database config file: \"{}\"", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
}

std::string Database::path() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_path;
}

bool Database::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "database" })) {
        spdlog::error("Invalid database JSON: [{}]", jConfig.dump());
        return false;
    }

    auto jDatabase = jConfig["database"];
    if (!CheckJsonMessageForKeys(jDatabase, { "path" })) {
        spdlog::error("Invalid database JSON: [{}]", jDatabase.dump());
        return false;
    }

    m_path = jDatabase["path"].get<std::string>();
    spdlog::info("Database: path={}",
                                 m_path);

    return true;
}

}
}
