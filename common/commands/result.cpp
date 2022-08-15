#include "result.h"

namespace command {

Result::Result()
 : m_isValid(false)
 , m_type(Type::Undefined)
 , m_command(nullptr)
{

}

Result::Result(const Command *command, const std::string &result)
 : m_isValid(true)
 , m_type(Type::String)
 , m_string(result)
 , m_command(command)
{

}

Result::~Result()
{
    
}

Result::Type Result::type() const
{
    return m_type;
}

const std::string &Result::toString() const
{
    return m_string;
}

bool Result::isValid() const
{
    return m_isValid;
}

const Command *Result::command() const
{
    return m_command;
}

}
