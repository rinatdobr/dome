#include "db.h"

#include <iostream>
#include <sstream>
#include <sqlite3.h>

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
    select strftime('%Y/%m/%d %H:%M:%S',timestamp), value from table_name \
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
    if (sqlite3_open(path.c_str(), &m_dbHandler) != SQLITE_OK) {
        std::cout << "sqlite3_open error" << std::endl;
        return;
    }

    m_isValid = true;
}

Db::~Db()
{
    if (!m_isValid) {
        std::cout << "Io is not valid" << std::endl;
        return;
    }

    if (sqlite3_close(m_dbHandler) != SQLITE_OK) {
        std::cout << "sqlite3_close: " << sqlite3_errmsg(m_dbHandler) << std::endl;
    }
}

void Db::write(const command::Result &result)
{
    if (!m_isValid) {
        std::cout << "Io is not valid" << std::endl;
        return;
    }

    if (!result.isValid()) {
        std::cout << "No need to write not valid result" << std::endl;
        return;
    }

    const std::string tableName(result.command()->name());
    if (!checkIfTableExists(tableName) &&
        !createTable(tableName)) {
        std::cout << "No table to write value" << std::endl;
        return;
    }

    writeValue(tableName, result.toString());

    std::cout << "write" << std::endl;
}

std::string Db::readLastForSec(const command::Command &command, uint seconds)
{
    if (!m_isValid) {
        std::cout << "Io is not valid" << std::endl;
        return {};
    }

    const std::string tableName(command.name());
    if (!checkIfTableExists(tableName)) {
        std::cout << "No table to write value" << std::endl;
        return {};
    }

    return readLastValuesForSec(tableName, seconds);
}

bool Db::checkIfTableExists(const std::string &tableName)
{
    sqlite3_stmt *sqlRequest;
    int sqlCode = sqlite3_prepare_v2(m_dbHandler, CheckIfTableExists.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlite3_prepare_v2: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return false;
    }

    sqlCode = sqlite3_bind_text(sqlRequest, 1, tableName.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlCode: " << sqlite3_errmsg(m_dbHandler) << std::endl;
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
        std::cout << "sqlite3_step: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return false;
    }
}

bool Db::createTable(const std::string &tableName)
{
    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string createTable(CreateTable);
    createTable.replace(createTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);

    std::cout << createTable << std::endl;

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, createTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlite3_prepare_v2: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return false;
    }

    sqlCode = sqlite3_step(sqlRequest);
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return true;
    }
    else {
        std::cout << "sqlite3_step: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return false;
    }
}

void Db::writeValue(const std::string &tableName, const std::string &value)
{
    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string insertToTable(InsertToTable);
    insertToTable.replace(insertToTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);

    std::cout << insertToTable << std::endl;

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, insertToTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlite3_prepare_v2: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return;
    }

    sqlCode = sqlite3_bind_text(sqlRequest, 1, value.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlCode: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return;
    }

    sqlCode = sqlite3_step(sqlRequest);
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return;
    }
    else {
        std::cout << "sqlite3_step: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return;
    }
}

std::string Db::readLastValuesForSec(const std::string &tableName, uint seconds)
{
    sqlite3_stmt *sqlRequest;
    std::string tableNameToReplace("table_name");
    std::string secondsValueToReplace("seconds_value");
    std::string readFromTable(ReadFromTableForSec);
    readFromTable.replace(readFromTable.find(tableNameToReplace), tableNameToReplace.length(), tableName);
    readFromTable.replace(readFromTable.find(secondsValueToReplace), secondsValueToReplace.length(), std::to_string(seconds));

    std::cout << readFromTable << std::endl;

    int sqlCode = sqlite3_prepare_v2(m_dbHandler, readFromTable.c_str(), -1, &sqlRequest, NULL);
    if (sqlCode != SQLITE_OK) {
        std::cout << "sqlite3_prepare_v2: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return {};
    }

    // sqlCode = sqlite3_bind_int64(sqlRequest, 1, seconds);
    // if (sqlCode != SQLITE_OK) {
    //     std::cout << "sqlCode: " << sqlite3_errmsg(m_dbHandler) << std::endl;
    //     sqlite3_finalize(sqlRequest);
    //     return {};
    // }

    // std::cout << sqlite3_sql(sqlRequest) << std::endl;

    std::ostringstream result;
    while ((sqlCode = sqlite3_step(sqlRequest)) == SQLITE_ROW) {
        result << sqlite3_column_text(sqlRequest, 0) << " | " << sqlite3_column_text(sqlRequest, 1) << "\n";
    }
    if (sqlCode == SQLITE_DONE) {
        sqlite3_finalize(sqlRequest);
        return result.str();
    }
    else {
        std::cout << "sqlite3_step: " << sqlite3_errmsg(m_dbHandler) << std::endl;
        sqlite3_finalize(sqlRequest);
        return {};
    }
}

}
}
