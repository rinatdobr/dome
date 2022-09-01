#include "result.h"

#include <spdlog/spdlog.h>

namespace command {

Result::Result()
 : m_isValid(false)
 , m_type(Type::Undefined)
 , m_command(nullptr)
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Result::Result(const Command *command, Type type, const std::string &result)
 : m_isValid(true)
 , m_type(type)
 , m_string(result)
 , m_command(command)
{
    spdlog::trace("{}:{} {} type={} result={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, type, result);

    switch (m_type) {
        case Type::String:
            // nothing
        break;
        case Type::Photo:
            // nothing?
        break;
    }
}

Result::~Result()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);
}

Result::Type Result::type() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_type;
}

const std::string &Result::toString() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_string;
}

bool Result::isValid() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_isValid;
}

const Command *Result::command() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_command;
}

}
