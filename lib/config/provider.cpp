#include "provider.h"

#include "utils.h"
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
    spdlog::trace("{}:{} {} str={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, str);

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

void Provider::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    m_id = jConfig["id"].get<std::string>();
    m_location = jConfig["location"].get<std::string>();
    m_periodSec = PeriodToSeconds(jConfig["period"].get<std::string>());
    m_savePeriodSec = PeriodToSeconds(jConfig["save_period"].get<std::string>());
    m_maxFrequenceSec = PeriodToSeconds(jConfig["max_frequence"].get<std::string>());
    spdlog::info("Data provider: id={} location={} period={}", m_id, m_location, m_periodSec);

    auto jSources = jConfig["sources"];
    for (const auto &jSource : jSources) {
        if (jSource.size() != 3) {
            spdlog::error("Invalid source format: \" {} \"", jSource.dump());
            continue;
        }

        Source source;
        source.id = jSource["id"].get<std::string>();
        source.type = Source::StrToType(jSource["type"].get<std::string>());
        source.dataType = StrToDataType(jSource["data_type"].get<std::string>());

        spdlog::info("Data provider config: id={} type={} data_type={}",
            source.id, source.type, source.dataType);

        m_sources.push_back(source);
    }

    auto jRequests = jConfig["commands"];
    for (const auto &jRequest : jRequests) {
        Request request;
        request.name = jRequest;
        m_requests.push_back(request);
    }
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

    return m_requests;
}

}
}
