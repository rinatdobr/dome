#include "db.h"

#include <sstream>
#include <sqlite3.h>
#include <spdlog/spdlog.h>

namespace {

const std::string CheckIfTableExists("\
    select 1 from sqlite_master \
    where type='table' and name=?"
);

const std::string CreateTable("\
    create table table_name (\
        id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
        timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, \
        value TEXT\
    )"
);

const std::string InsertToTable("\
    insert into table_name (value) \
    values (?)"
);

const std::string ReadFromTableForSec("\
    select strftime('%Y/%m/%d %H:%M:%S',timestamp,'localtime'), value from table_name \
    where timestamp >= (select DATETIME('now', '-seconds_value second'))"
);

}

namespace dome {
namespace io {

Db::Db(const std::string path)
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

Db::~Db()
{
    spdlog::trace("{}:{} {}", __FILE__, __LINE__, __PRETTY_FUNCTION__);

    if (!m_isValid) {
        return;
    }

    if (sqlite3_close(m_dbHandler) != SQLITE_OK) {
        spdlog::error("Can't close DB: {}", sqlite3_errmsg(m_dbHandler));
    }
}

void Db::write(const command::Result &result)
{
    spdlog::trace("{}:{} {} result={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, result.isValid());

    if (!m_isValid) {
        spdlog::error("DB is not valid to write");
        return;
    }

    if (!result.isValid()) {
        spdlog::warn("No need to write not valid result");
        return;
    }

    const std::string tableName(result.command()->name());
    if (!checkIfTableExists(tableName) &&
        !createTable(tableName)) {
        spdlog::error("No table to write result");
        return;
    }

    writeValue(tableName, result.toString());
}

std::string Db::readLastForSec(const command::Command &command, uint seconds)
{
    spdlog::trace("{}:{} {} command={} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, command.name(), seconds);

    if (!m_isValid) {
        spdlog::error("DB is not valid to read");
        return {};
    }

    const std::string tableName(command.name());
    if (!checkIfTableExists(tableName)) {
        spdlog::error("No table to read result");
        return {};
    }

    return readLastValuesForSec(tableName, seconds);
}

bool Db::checkIfTableExists(const std::string &tableName)
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

bool Db::createTable(const std::string &tableName)
{
    spdlog::trace("{}:{} {} tableName={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName);

    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string createTable(CreateTable);
    createTable.replace(createTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, createTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_prepare_v2: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return false;
    }

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

void Db::writeValue(const std::string &tableName, const std::string &value)
{
    spdlog::trace("{}:{} {} tableName={} value={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, value);

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

    sqlCode = sqlite3_bind_text(sqlRequest, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_bind_text: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return;
    }

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

std::string Db::readLastValuesForSec(const std::string &tableName, uint seconds)
{
    spdlog::trace("{}:{} {} tableName={} seconds={}", __FILE__, __LINE__, __PRETTY_FUNCTION__, tableName, seconds);

    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string secondsValueToReplace("seconds_value");
    std::string readFromTable(ReadFromTableForSec);
    readFromTable.replace(readFromTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);
    readFromTable.replace(readFromTable.find(secondsValueToReplace), secondsValueToReplace.length(), std::to_string(seconds));

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, readFromTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        spdlog::error("sqlite3_prepare_v2: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return {};
    }

    // sqlCode = sqlite3_bind_int64(sqlRequest, 1, seconds);
    // if (sqlCode != SQLITE_OK) {
    //     sqlite3_finalize(sqlRequest);
    //     return {};
    // }

    std::ostringstream result;
    while ((sqlCode = sqlite3_step(sqlRequest)) == SQLITE_ROW) {
        result << sqlite3_column_text(sqlRequest, 0) << " | " << sqlite3_column_text(sqlRequest, 1) << "\n";
    }
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return result.str();
    }
    else {
        spdlog::error("sqlite3_step: {}", sqlite3_errmsg(m_dbHandler));
        sqlite3_finalize(sqlRequest);
        return {};
    }
}

}
}
