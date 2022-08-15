#ifndef DB_WRITER_H
#define DB_WRITER_H

#include <memory>
#include <sqlite3.h>

#include <commands/command.h>
#include <commands/result.h>

#include "io.h"

namespace dome {
namespace io {

class Db : public Io
{
public:
    Db(const std::string path);
    ~Db();

    void write(const command::Result &result) override;
    std::string readLastForSec(const command::Command &command, uint seconds) override;

private:
    std::string m_path;
    bool m_isValid;
    sqlite3 *m_dbHandler;

    bool checkIfTableExists(const std::string &tableName);
    bool createTable(const std::string &tableName);
    void writeValue(const std::string &tableName, const std::string &value);
    std::string readLastValuesForSec(const std::string &tableName, uint seconds);
};

}
}

#endif
