#include "endpoint.h"

#include "utils/utils.h"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace dome {
namespace config {

EndPoint::EndPoint()
    : File("")
    , m_location("undefined")
    , m_isRequestable(false)
    , m_isSavable(false)
    , m_isProviderable(false)
    , m_isReplyable(false)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    I_am_not_valid();
}

EndPoint::EndPoint(std::string path)
    : File(path)
    , m_location("undefined")
    , m_isRequestable(false)
    , m_isSavable(false)
    , m_isProviderable(false)
    , m_isReplyable(false)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            path);

    if (!isValid()) {
        return;
    }

    if (!parse()) {
        spdlog::error("Can't parse endpoint config file: {}", path);
        I_am_not_valid();
        return;
    }

    I_am_valid();
}

std::string EndPoint::id() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_id;
}

std::string EndPoint::location() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_location;
}

bool EndPoint::requestable() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isRequestable;
}

bool EndPoint::gettable() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isGettable;
}

bool EndPoint::savable() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isSavable;
}

bool EndPoint::providerable() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isProviderable;
}

bool EndPoint::replyable() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isReplyable;
}

uint EndPoint::periodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_periodSec;
}

uint EndPoint::savePeriodSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_savePeriodSec;
}

uint EndPoint::maxFrequenceSec() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_maxFrequenceSec;
}

const std::vector<Source> &EndPoint::sources() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_sources;
}

bool EndPoint::parse()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    auto configData = read();
    nlohmann::json jConfig = nlohmann::json::parse(configData);
    if (!CheckJsonMessageForKeys(jConfig, { "id", "period", "sources" })) {
        spdlog::error("Invalid endpoint JSON: [{}]", jConfig.dump());
        return false;   
    }

    m_id = jConfig["id"].get<std::string>();
    if (CheckJsonMessageForKeys(jConfig, { "location" })) {
        m_location = jConfig["location"].get<std::string>();
    }
    spdlog::info("Data endpoint: id={} location={}",
                                 m_id, m_location);

    auto jSources = jConfig["sources"];
    for (const auto &jSource : jSources) {
        if (!CheckJsonMessageForKeys(jSource, { "id", "type", "data_type" })) {
            spdlog::error("Invalid source JSON: [{}]", jSource.dump());
            continue;
        }

        Source source(jSource["id"].get<std::string>(), jSource["type"].get<std::string>(), jSource["data_type"].get<std::string>());
        spdlog::info("Data endpoint source: id={} type={} data_type={}",
                                            source.id(), source.type(), source.dataType());

        m_sources.push_back(source);
    }

    if (CheckJsonMessageForKeys(jConfig, { "requestable" })) {
        m_isRequestable = true;
        auto jRequestable = jConfig["requestable"];
        if (CheckJsonMessageForKeys(jRequestable, { "get" })) {
            m_isGettable = jConfig["get"].get<bool>();
        }
    }

    if (CheckJsonMessageForKeys(jConfig, { "savable" })) {
        m_isSavable = true;
        auto jSavable = jConfig["savable"];
        if (CheckJsonMessageForKeys(jSavable, { "period" })) {
            m_savePeriodSec = PeriodToSeconds(jSavable["period"].get<std::string>());
        }
    }

    if (CheckJsonMessageForKeys(jConfig, { "providerable" })) {
        m_isProviderable = true;
        auto jProviderable = jConfig["providerable"];
        if (CheckJsonMessageForKeys(jProviderable, { "period", "max_frequence" })) {
            m_periodSec = PeriodToSeconds(jProviderable["period"].get<std::string>());
            m_maxFrequenceSec = PeriodToSeconds(jProviderable["max_frequence"].get<std::string>());
        }
    }

    if (CheckJsonMessageForKeys(jConfig, { "replyable" })) {
        m_isReplyable = jConfig["replyable"].get<bool>();
    }

    return true;
}

}
}
