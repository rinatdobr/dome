#ifndef DB_WRITER_H
#define DB_WRITER_H

#include <string>
#include <memory>
#include <sqlite3.h>

#include <config/provider.h>

namespace dome {
namespace db {

union value
{
    double doubleV;
};


class Writer
{
public:
    Writer(const std::string &path);
    ~Writer();

    void write(const std::string &tableName, double value);

private:
    std::string m_path;
    bool m_isValid;
    sqlite3 *m_dbHandler;

    bool checkIfTableExists(const std::string &tableName);
    bool createTable(const std::string &tableName, dome::config::Source::DataType dataType);
    void writeValue(const std::string &tableName, const union value& v, dome::config::Source::DataType dataType);
};

}
}

#endif
