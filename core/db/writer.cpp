#include "writer.h"

#include <sqlite3.h>
#include <spdlog/spdlog.h>

namespace {

const std::string CheckIfTableExists("\
    select 1 from sqlite_master \
    where type='table' and name=?"
);

const std::string CreateTable("\
    create table 'table_name' (\
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
        value value_type\
    )"
);

const std::string InsertToTable("\
    insert into 'table_name' (value) \
    values (?)"
);

}

namespace dome {
namespace db {

Writer::Writer(const std::string &path)
    : m_path(path)
    , m_isValid(false)
    , m_dbHandler(nullptr)
{
    spdlog::trace("{}:{} {} path={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, path);

    if (sqlite3_open(path.c_str(), &m_dbHandler) != SQLITE_OK) {
        spdlog::error("Can't open DB: {}", sqlite3_errmsg(m_dbHandler));
        return;
    }

    m_isValid = true;
}

Writer::~Writer()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!m_isValid) {
        return;
    }

    if (sqlite3_close(m_dbHandler) != SQLITE_OK) {
        spdlog::error("Can't close DB: {}", sqlite3_errmsg(m_dbHandler));
    }
}

void Writer::write(const std::string &tableName, double value)
{
    spdlog::trace("{}:{} {} tableName={} value={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, value);

    if (!m_isValid) {
        spdlog::error("DB is not valid to write");
        return;
    }

    if (!checkIfTableExists(tableName) &&
        !createTable(tableName, dome::config::Source::DataType::Float)) {
        spdlog::error("No table to write result");
        return;
    }

    writeValue(tableName, {.doubleV = value}, dome::config::Source::DataType::Float);
}

void Writer::write(const std::string &tableName, int value)
{
    spdlog::trace("{}:{} {} tableName={} value={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, value);

    if (!m_isValid) {
        spdlog::error("DB is not valid to write");
        return;
    }

    if (!checkIfTableExists(tableName) &&
        !createTable(tableName, dome::config::Source::DataType::Int)) {
        spdlog::error("No table to write result");
        return;
    }

    writeValue(tableName, {.intV = value}, dome::config::Source::DataType::Int);
}

bool Writer::checkIfTableExists(const std::string &tableName)
{
    spdlog::trace("{}:{} {} tableName={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName);

    sqlite3_stmt *sqlRequest;
    int sqlCode = sqlite3_prepare_v2(m_dbHandler, CheckIfTableExists.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_prepare_v2: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }

    sqlCode = sqlite3_bind_text(sqlRequest, 1, tableName.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_bind_text: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }

    spdlog::trace("sqlite3_expanded_sql: {}", sqlite3_expanded_sql(sqlRequest));

    sqlCode = sqlite3_step(sqlRequest);
    if (sqlCode == SQLITE_ROW) {
        sqlite3_finalize(sqlRequest);
        return true;
    }
    else if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return false;
    }
    else {
        spdlog::error("sqlite3_step: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }
}

bool Writer::createTable(const std::string &tableName, dome::config::Source::DataType dataType)
{
    spdlog::trace("{}:{} {} tableName={} dataType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, dataType);

    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string valueTypeToReplace("value_type");
    std::string createTable(CreateTable);
    createTable.replace(createTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);
    switch (dataType) {
        case dome::config::Source::DataType::Float:
            createTable.replace(createTable.find(valueTypeToReplace), valueTypeToReplace.length(), "REAL");
        break;
        case dome::config::Source::DataType::Int:
	    createTable.replace(createTable.find(valueTypeToReplace), valueTypeToReplace.length(), "INTEGER");
	break;
    }

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, createTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_prepare_v2: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }

    spdlog::trace("sqlite3_expanded_sql: {}", sqlite3_expanded_sql(sqlRequest));

    sqlCode = sqlite3_step(sqlRequest);
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return true;
    }
    else {
        spdlog::error("sqlite3_step: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }
}

void Writer::writeValue(const std::string &tableName, const union value& v, dome::config::Source::DataType dataType)
{
    spdlog::trace("{}:{} {} tableName={} dataType={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, dataType);

    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string insertToTable(InsertToTable);
    insertToTable.replace(insertToTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, insertToTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_prepare_v2: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return;
    }

    switch (dataType) {
        case dome::config::Source::DataType::Float:
            sqlCode = sqlite3_bind_double(sqlRequest, 1, v.doubleV);
            if (sqlCode != SQLITE_OK) {
                spdlog::error("sqlite3_bind_text: {}", sqlite3_errmsg(m_dbHandler));
                sqlite3_finalize(sqlRequest);
                return;
            }
        break;
        case dome::config::Source::DataType::Int:
            sqlCode = sqlite3_bind_int(sqlRequest, 1, v.intV);
            if (sqlCode != SQLITE_OK) {
                spdlog::error("sqlite3_bind_text: {}", sqlite3_errmsg(m_dbHandler));
                sqlite3_finalize(sqlRequest);
                return;
            }
        break;
    }

    spdlog::trace("sqlite3_expanded_sql: {}", sqlite3_expanded_sql(sqlRequest));

    sqlCode = sqlite3_step(sqlRequest);
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return;
    }
    else {
        spdlog::error("sqlite3_step: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return;
    }
}

}
}
