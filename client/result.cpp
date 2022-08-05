#include "result.h"

Result::Result()
 : m_isValid(false)
 , m_type(Type::Undefined)
{

}

Result::Result(const std::string &result)
 : m_isValid(true)
 , m_type(Type::String)
 , m_string(result)
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
