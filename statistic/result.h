#ifndef RESULT_H
#define RESULT_H

#include <vector>
#include <string>
#include <memory>

#include <commands/command.h>

class Result
{
public:
    Result(std::unique_ptr<command::Result> &&result, std::vector<std::string> columns);

private:
    std::unique_ptr<command::Result> &&m_result;
    std::vector<std::string> m_columns;
};

#endif
