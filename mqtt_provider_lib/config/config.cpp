#include "config.h"

#include "utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Source::Type StrToSourceType(const std::string& str)
{
    if (str == "temperature") {
        return Source::Type::Temperature;
    }
    else if (str == "humidity") {
        return Source::Type::Humidity;
    }

    return Source::Type::Undefined;
}

Source::DataType StrToDataType(const std::string& str)
{
    if (str == "float") {
        return Source::DataType::Float;
    }

    return Source::DataType::Undefined;
}

Config::Config(std::string path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
        path);

    parse();
}

std::string Source::TypeToStr(Source::Type type)
{
    spdlog::trace("{}:{} {} type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, type);
    
    switch (type) {
        case Source::Type::Undefined:
            return "Undefined";
        case Source::Type::Temperature:
            return "Temperature";
        case Source::Type::Humidity:
            return "Humidity";
    }

    return "Undefined";
}

void Config::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    m_id = jConfig["id"].get<std::string>();
    m_periodSec = PeriodToSeconds(jConfig["period"].get<std::string>());
    spdlog::info("Data provider: id={} period={}", m_id, m_periodSec);

    auto jSources = jConfig["sources"];
    for(const auto &jSource : jSources) {
        if (jSource.size() != 5) {
            spdlog::error("Invalid source format: \" {} \"", jSource.dump());
            continue;
        }

        Source source;
        source.name = jSource["name"].get<std::string>();
        source.description = jSource["description"].get<std::string>();
        source.location = jSource["location"].get<std::string>();
        source.type = StrToSourceType(jSource["type"].get<std::string>());
        source.dataType = StrToDataType(jSource["data_type"].get<std::string>());

        spdlog::info("Data provider config: name={} description={} location={} type={} data_type={}",
            source.name, source.description, source.location, source.type, source.dataType);

        m_sources.push_back(source);
    }


}

std::string Config::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

uint Config::periodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_periodSec;
}

const std::vector<Source> &Config::sources() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_sources;
}


}
}
