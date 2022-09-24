#ifndef COMMANDS_RESULT_H
#define COMMANDS_RESULT_H

#include <string>
#include <filesystem>

namespace command {

class Command;

class Result
{
public:
enum class Type
{
    Undefined,
    String,
    File,
    Error
};

enum class FileType
{
    Undefined,
    Photo,
    Document
};

enum class Status
{
    Success,
    Fail
};

    Result(const std::string &result);
    Result(const Command *command, const std::string &result, Status status);
    Result(const Command *command, const std::filesystem::path &result, FileType fileType, Status status);
    ~Result();

    Type type() const;
    FileType fileType() const;
    const std::string &toString() const;
    bool isValid() const;
    const Command *command() const;
    std::string errorMessage() const;

protected:
    bool m_isValid;
    Type m_type;
    FileType m_fileType;

    std::string m_string;
    const Command *m_command;
};

}

#endif
