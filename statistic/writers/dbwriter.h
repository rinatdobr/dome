#ifndef DB_WRITER_H
#define DB_WRITER_H

#include <memory>
#include <sqlite3.h>

#include <commands/result.h>

#include "writer.h"

class DbWriter : public Writer
{
public:

    DbWriter(const std::string path);

    void write(const std::unique_ptr<command::Command> &command, const command::Result &result) override;

private:
    std::string m_path;
    bool m_isValid;
    sqlite3 *m_handler;
};

#endif
