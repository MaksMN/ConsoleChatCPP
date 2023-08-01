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

    int SQL_RESULT_LEN = 240;
    int SQL_RETURN_CODE_LEN = 1024;

public:
    ~ODBC() = default;
    void initialize();
    void hello();
    std::shared_ptr<User> getUserByID(const ullong &userID, uint &db_error_number) override;
    std::shared_ptr<User> getUserByLogin(const std::string &userLogin, uint &db_error_number) override;
    ullong getCount(std::string table, std::string where, uint &db_error_number) override;
    std::string userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;
    bool saveUser(std::shared_ptr<User> user, uint &db_error_number) override;
    bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number) override;
    std::shared_ptr<Message> getMessageByID(const ullong &messageID, uint &db_error_number) override;

    bool addMessage(std::shared_ptr<Message> &message, uint &db_error_number) override;
    std::string messageList(ullong &reader_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;
    std::string messageList(ullong &reader_id, ullong interlocutor_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;

private:
    int dbQuery(std::string &query, uint &db_error_number);

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
