#ifndef RESULT_H
#define RESULT_H

#include <string>


class Result
{
public:
    enum class Type
    {
        Undefined,
        String
    };

    Result();
    explicit Result(const std::string &result);
    ~Result();

    Type type() const;
    const std::string &toString() const;
    bool isValid() const;

protected:
    bool m_isValid;
    Type m_type;

    std::string m_string;
};

#endif
