#ifndef IO_DB_H
#define IO_DB_H

#include <memory>
#include <sqlite3.h>

#include <config/db.h>
#include <commands/result.h>

#include "io.h"

namespace dome {
namespace io {

class Db : public Io
{
public:
    static std::map<std::string, std::shared_ptr<Db>> Create(const std::map<std::string, dome::config::Db::Config> &dbConfig);

    Db(const std::string &name, const std::string &path);
    ~Db();

    std::string name() const;
    void write(const command::Result &result) override;
    std::string readLastForSec(const std::string &name, uint seconds, Type type) override;

private:
    std::string m_name;
    std::string m_path;
    bool m_isValid;
    sqlite3 *m_dbHandler;

    bool checkIfTableExists(const std::string &tableName);
    bool createTable(const std::string &tableName);
    void writeValue(const std::string &tableName, const std::string &value);
    std::string readLastValuesForSec(const std::string &tableName, uint seconds, Type type);
    std::string readLastValuesForSecChart(const std::string &tableName, uint seconds);
    std::string readLastValuesForSecData(const std::string &tableName, uint seconds);
};

}
}

#endif
