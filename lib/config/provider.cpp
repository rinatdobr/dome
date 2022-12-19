#include "provider.h"

#include "utils/utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

Source::DataType StrToDataType(const std::string& str)
{
    if (str == "float") {
        return Source::DataType::Float;
    }
    else if (str == "path") {
        return Source::DataType::Path;
    }
    else if (str == "int") {
        return Source::DataType::Int;
    }

    return Source::DataType::Undefined;
}

std::string Source::TypeToStr(Source::Type type)
{
    spdlog::trace("{}:{} {}, type={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            type);
    
    switch (type) {
        case Source::Type::Undefined:
            return "undefined";
        case Source::Type::Temperature:
            return "temperature";
        case Source::Type::Humidity:
            return "humidity";
        case Source::Type::IpCamera:
            return "ip_camera";
        case Source::Type::Co2:
            return "co2";
    }

    return "undefined";
}

Source::Type Source::StrToType(const std::string &str)
{
    spdlog::trace("{}:{} {}, str={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            str);

    if (str == "temperature") {
        return Source::Type::Temperature;
    }
    else if (str == "humidity") {
        return Source::Type::Humidity;
    }
    else if (str == "request") {
        return Source::Type::Request;
    }
    else if (str == "ip_camera") {
        return Source::Type::IpCamera;
    }
    else if (str == "co2") {
        return Source::Type::Co2;
    }

    return Source::Type::Undefined;
}

Provider::Provider()
    : File("")
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    I_am_not_valid();
}

Provider::Provider(std::string path)
    : File(path)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse provider config file: {}", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
}

std::string Provider::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

std::string Provider::location() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_location;
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

uint Provider::maxFrequenceSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_maxFrequenceSec;
}

const std::vector<Source> &Provider::sources() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_sources;
}

const std::vector<Request> &Provider::requests() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_messages;
}

bool Provider::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "id", "location", "period", "save_period", "max_frequence", "sources", "requests" })) {
        spdlog::error("Invalid provider JSON: [{}]", jConfig.dump());
        return false;   
    }

    m_id = jConfig["id"].get<std::string>();
    m_location = jConfig["location"].get<std::string>();
    m_periodSec = PeriodToSeconds(jConfig["period"].get<std::string>());
    m_savePeriodSec = PeriodToSeconds(jConfig["save_period"].get<std::string>());
    m_maxFrequenceSec = PeriodToSeconds(jConfig["max_frequence"].get<std::string>());
    spdlog::info("Data provider: id={} location={} period={} save_period={} max_frequence={}",
                                 m_id, m_location, m_periodSec, m_savePeriodSec, m_maxFrequenceSec);

    auto jSources = jConfig["sources"];
    for (const auto &jSource : jSources) {
        if (!CheckJsonMessageForKeys(jSource, { "id", "type", "data_type" })) {
            spdlog::error("Invalid source JSON: [{}]", jSource.dump());
            continue;
        }

        Source source;
        source.id = jSource["id"].get<std::string>();
        source.type = Source::StrToType(jSource["type"].get<std::string>());
        source.dataType = StrToDataType(jSource["data_type"].get<std::string>());
        spdlog::info("Data provider source: id={} type={} data_type={}",
                                            source.id, source.type, source.dataType);

        m_sources.push_back(source);
    }

    auto jRequests = jConfig["requests"];
    for (const auto &jRequest : jRequests) {
        Request request;
        request.name = jRequest;
        spdlog::info("Data provider command: name={}",
                                             request.name);
        
        m_messages.push_back(request);
    }

    return true;
}

}
}
