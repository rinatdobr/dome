#include "provider.h"

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

Provider::Provider()
    : File("")
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Provider::Provider(std::string path)
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

void Provider::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    m_id = jConfig["id"].get<std::string>();
    m_periodSec = PeriodToSeconds(jConfig["period"].get<std::string>());
    m_savePeriodSec = PeriodToSeconds(jConfig["save_period"].get<std::string>());
    spdlog::info("Data provider: id={} period={}", m_id, m_periodSec);

    auto jSources = jConfig["sources"];
    for (const auto &jSource : jSources) {
        if (jSource.size() != 5) {
            spdlog::error("Invalid source format: \" {} \"", jSource.dump());
            continue;
        }

        Source source;
        source.name = jSource["name"].get<std::string>();
        source.location = jSource["location"].get<std::string>();
        source.type = StrToSourceType(jSource["type"].get<std::string>());
        source.dataType = StrToDataType(jSource["data_type"].get<std::string>());

        spdlog::info("Data provider config: name={} location={} type={} data_type={}",
            source.name, source.location, source.type, source.dataType);

        m_sources.push_back(source);
    }

    auto jCommands = jConfig["commands"];
    for (const auto &jCommand : jCommands) {
        Command command;
        command.name = jCommand;
        m_commands.push_back(command);
    }
}

std::string Provider::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

uint Provider::periodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_periodSec;
}

uint Provider::savePeriodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_savePeriodSec;
}

const std::vector<Source> &Provider::sources() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_sources;
}

const std::vector<Command> &Provider::commands() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_commands;
}

}
}
