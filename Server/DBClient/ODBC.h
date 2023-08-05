#pragma once
#if defined(_WIN64) || defined(_WIN32)
#include "DBCore.h"
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <exception>

class ODBC final : public DBCore
{
private:
    std::string connect_data;

    SQLHANDLE sqlConnHandle{nullptr}; // дескриптор для соединения с базой данных
    SQLHANDLE sqlEnvHandle{nullptr};  // дескриптор окружения базы данных
    SQLHANDLE sqlStmtHandle{nullptr}; // дескриптор для выполнения запросов к базе данных

    int SQL_RESULT_LEN = 240;
    int SQL_RETURN_CODE_LEN = 1024;

public:
    ~ODBC() = default;
    /// @brief инициализация подключения к базе данных.
    bool initialize() override;

    /// @brief Получает указатель пользователя по ID
    /// @param userID
    /// @param db_error_number
    /// @return
    std::shared_ptr<User> getUserByID(const ullong &userID, uint &db_error_number) override;

    /// @brief Получает указатель пользователя по логину
    /// @param userLogin
    /// @param db_error_number
    /// @return
    std::shared_ptr<User> getUserByLogin(const std::string &userLogin, uint &db_error_number) override;

    /// @brief Получает количество элементов с запросе SELECT COUNT(*)
    /// @param table
    /// @param where
    /// @param db_error_number
    /// @return
    ullong getCount(std::string table, std::string where, uint &db_error_number) override;

    /// @brief Выводит список пользователей в строку
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_error_number
    /// @return
    std::string userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;

    /// @brief Сохраняет пользователя если он есть в базе
    /// @param user
    /// @param login_busy true если логин занят. Сохранение отменяется.
    /// @param email_busy true если email занят. Сохранение отменяется.
    /// @param db_error_number
    /// @return
    bool saveUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number) override;

    /// @brief Добавляет нового пользователя в базу
    /// @param user
    /// @param login_busy true если логин занят. Добавление отменяется.
    /// @param email_busy true если email занят. Добавление отменяется.
    /// @param db_error_number
    /// @return
    bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number) override;

    /// @brief Получает сообщение по ID. Не работает. Возможно будет удалена.
    /// @param messageID
    /// @param db_error_number
    /// @return
    std::shared_ptr<Message> getMessageByID(const ullong &messageID, uint &db_error_number) override;

    /// @brief Добавляет сообщение в базу данных
    /// @param message
    /// @param db_error_number
    /// @return
    bool addMessage(std::shared_ptr<Message> &message, uint &db_error_number) override;

    /// @brief Выводит список публичных сообщений
    /// @param reader_id Если читатель не является автором, сообщение будет помечено прочитанным.
    /// @param start
    /// @param per_page
    /// @param capacity выводит количество сообщений в списке
    /// @param db_error_number
    /// @return
    std::string messageList(ullong &reader_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;

    /// @brief Выводит список личных сообщений
    /// @param reader_id
    /// @param interlocutor_id
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_error_number
    /// @return
    std::string messageList(ullong &reader_id, ullong interlocutor_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;

    /// @brief удаление элемента по ID
    /// @param id
    /// @param table
    /// @param db_error_number
    /// @return
    bool deleteByID(ullong &id, std::string &table, uint &db_error_number) override;

    /// @brief Изменяет битовые флаги статуса
    /// @param id
    /// @param table
    /// @param add
    /// @param db_error_number
    /// @return
    bool setStatus(ullong &id, std::string &table, bool add, uint &db_error_number) override;

    void DBclose() override;

    /// @brief тестовая функция, указывает какой тип подключения к базе
    void hello() override;

private:
    std::shared_ptr<User> fetchUserRow(uint &db_error_number, uint startCol = 1, bool getPassData = true);
    void fetchMessageRow(std::shared_ptr<User> &user, std::shared_ptr<Message> &msg, uint &db_error_number, uint startCol = 1, bool pub = true);

    /// @brief Выполняет запрос
    /// @param query
    /// @param db_error_number
    /// @return количество обработанных рядов или -1 если операция не затрагивает ряды.
    int dbQuery(std::string &query, uint &db_error_number);

    SQLINTEGER Fetch(uint &db_error_number);

    void BindCol(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber,
        SQLSMALLINT TargetType,
        SQLPOINTER TargetValuePtr,
        SQLLEN BufferLength,
        SQLLEN *StrLen_or_IndPtr);

    void diagInfo(SQLINTEGER handle_type, SQLHANDLE &handle, const std::string &function, const std::string &query);

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

class BindColException : public std::exception
{
public:
    virtual const char *what() const noexcept override
    {
        return "ERROR: SQLBindColException!";
    }
};

#endif