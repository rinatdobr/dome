#include "topic.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace mosq {

Topic::Topic(const std::string name, const dome::config::EndPoint &endPointConfig, std::vector<dome::message::Processor*> &processors)
    : m_name(name)
    , m_endPointConfig(endPointConfig)
    , m_processors(processors)
{
    spdlog::trace("{}:{} {} name={}", __FILE__, __LINE__, __PRETTY_FUNCTION__,
                            name);

}

Topic::~Topic()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

const std::string Topic::name() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_name;
}

const dome::config::EndPoint &Topic::endPointConfig() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_endPointConfig;
}

std::vector<dome::message::Processor*> Topic::processors() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_processors;
}

std::vector<std::string> Topic::GetTopicNames(const std::vector<Topic> &topics)
{
    std::vector<std::string> out;
    for (const auto& topic : topics) {
        out.push_back(topic.name());
    }

    return out;
}

}
}