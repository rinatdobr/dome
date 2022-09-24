#include "result.h"
#include "command.h"

#include <spdlog/spdlog.h>
#include <sstream>

namespace command {

Result::Result(const std::string &result)
 : m_isValid(true)
 , m_type(Type::Error)
 , m_string(result)
 , m_command(nullptr)
{
    spdlog::trace("{}:{} {} result={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, result);
}

Result::Result(const Command *command, const std::string &result, Status status)
 : m_isValid(true)
 , m_type(status == Status::Success ? Type::String : Type::Error)
 , m_string(result)
 , m_command(command)
{
    spdlog::trace("{}:{} {} result={} status={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, result, status);
}

Result::Result(const Command *command, const std::filesystem::path &result, FileType fileType, Status status)
 : m_isValid(true)
 , m_type(status == Status::Success ? Type::File : Type::Error)
 , m_fileType(fileType)
 , m_string(result)
 , m_command(command)
{
    spdlog::trace("{}:{} {} result={} type={} status={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, result.string(), fileType, status);
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

Result::FileType Result::fileType() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    return m_fileType;
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

std::string Result::errorMessage() const
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (m_command) {
        std::ostringstream stream;
        stream << m_command->name() << " : " << m_string;
        return stream.str();
    }
    else {
        return m_string;
    }
}


}
