#ifndef COMMANDS_RESULT_H
#define COMMANDS_RESULT_H

#include <string>

namespace command {

class Command;

class Result
{
public:
    enum class Type
    {
        Undefined,
        String,
        Photo,
        Document
    };

    Result();
    Result(const Command *command, Type type, const std::string &result);
    ~Result();

    Type type() const;
    const std::string &toString() const;
    bool isValid() const;
    const Command *command() const; 

protected:
    bool m_isValid;
    Type m_type;

    std::string m_string;
    const Command *m_command;
};

}

#endif
