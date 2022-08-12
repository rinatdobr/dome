#include "result.h"

Result::Result(std::unique_ptr<command::Result> &&result, std::vector<std::string> columns)
 : m_result(std::move(result))
 , m_columns(columns)
{

}
