#ifndef DB_WRITER_H
#define DB_WRITER_H

#include <string>
#include <memory>
#include <sqlite3.h>

#include "config/endpoint.h"
#include "utils/validatable.h"

namespace dome {
namespace db {

union value
{
    int intV;
    double doubleV;
};


class Writer : public dome::utils::Validatable
{
public:
    Writer(const std::string &path);
    ~Writer();

    void write(const std::string &tableName, double value);
    void write(const std::string &tableName, int value);

private:
    std::string m_path;
    sqlite3 *m_dbHandler;

    bool checkIfTableExists(const std::string &tableName);
    bool createTable(const std::string &tableName, dome::config::Source::DataType dataType);
    void writeValue(const std::string &tableName, const union value& v, dome::config::Source::DataType dataType);
};

}
}

#endif
