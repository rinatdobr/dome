#include "dbwriter.h"

#include <iostream>
#include <sqlite3.h>

DbWriter::DbWriter(const std::string path)
    : m_path(path)
    , m_isValid(false)
    , m_handler(nullptr)
{
    if (sqlite3_open(path.c_str(), &m_handler) != SQLITE_OK) {
        std::cout << "sqlite3_open error" << std::endl;
        return;
    }

    m_isValid = true;
}

void DbWriter::write(const std::unique_ptr<command::Command> &command, const command::Result &result)
{
    if (!m_isValid) {
        std::cout << "Writer is not valid" << std::endl;
    }

    if (!result.isValid()) {
        std::cout << "No need to write not valid result" << std::endl;
    }

    std::cout << "write" << std::endl;
}
