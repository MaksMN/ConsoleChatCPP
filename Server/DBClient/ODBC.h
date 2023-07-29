#pragma once
#include "DBCore.h"
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

class ODBC final : public DBCore
{
private:
    std::wstring connect_data;

    SQLHANDLE sqlConnHandle{nullptr}; // дескриптор для соединения с базой данных
    SQLHANDLE sqlEnvHandle{nullptr};  // дескриптор окружения базы данных
    SQLHANDLE sqlStmtHandle{nullptr}; // дескриптор для выполнения запросов к базе данных

public:
    ~ODBC() = default;
    void initialize();
    void hello();
    std::shared_ptr<User> getUserByID(const ullong &userID, uint &db_error_number) override;
    std::shared_ptr<User> getUserByLogin(const std::string &userLogin, uint &db_error_number) override;
    bool saveUser(std::shared_ptr<User> user, uint &db_error_number) override;
    bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number);

private:
    void wrongDescriptorMsg()
    {
        Misc::printMessage("Не удалось создать дескриптор ODBC");
    }
    void complete()
    {
        SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
        SQLDisconnect(sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
        SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
    }
};
