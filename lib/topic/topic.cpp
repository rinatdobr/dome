#include "topic.h"

#include <spdlog/spdlog.h>

namespace dome {
namespace topic {

Topic::Topic(const std::string name, const dome::config::Provider &config, std::vector<dome::message::Processor*> &processors)
    : m_name(name)
    , m_config(config)
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

const dome::config::Provider &Topic::config() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_config;
}

std::vector<dome::message::Processor*> Topic::processors() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_processors;
}

}
}