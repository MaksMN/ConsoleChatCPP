#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "../../Misc/Misc.h"
#include "../ChatCore/User.h"
#include "../ChatCore/Message.h"

#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

typedef unsigned int uint;
typedef unsigned long long ullong;

const extern char config_file[];
class DBCore
{
protected:
    const std::string server;
    const std::string port;
    const std::string dbuser;
    const std::string dbpass;
    const std::string dbname;
    const std::string db_character_set;

public:
    int provider;
    DBCore();
    virtual ~DBCore() = default;

    /// @brief Инициализация подключения к базе
    virtual void initialize() = 0;

    /// @brief Получить пользователя из базы по ID
    /// @param userID
    /// @param db_error_number
    /// @return
    virtual std::shared_ptr<User> getUserByID(const ullong &userID, uint &db_error_number) = 0;

    /// @brief Получить пользователя по логину
    /// @param userLogin
    /// @param db_error_number
    /// @return
    virtual std::shared_ptr<User> getUserByLogin(const std::string &userLogin, uint &db_error_number) = 0;

    /// @brief Получить количество рядов в запросе
    /// @param table
    /// @param where
    /// @param db_error_number
    /// @return
    virtual ullong getCount(std::string table, std::string where, uint &db_error_number) = 0;

    /// @brief Вывести список пользователей
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_error_number
    /// @return
    virtual std::string userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) = 0;

    /// @brief Сохранить пользователя если он уже есть в базе
    /// @param user
    /// @param db_error_number
    /// @return
    virtual bool saveUser(std::shared_ptr<User> user, uint &db_error_number) = 0;

    /// @brief Добавить нового пользователя в базу
    /// @param user
    /// @param login_busy
    /// @param email_busy
    /// @param db_error_number
    /// @return
    virtual bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number) = 0;

    /// @brief Получает сообщение из базы
    /// @param messageID
    /// @param db_error_number
    /// @return
    virtual std::shared_ptr<Message> getMessageByID(const ullong &messageID, uint &db_error_number) = 0;

    /// @brief Добавляет новое сообщение в базу
    /// @param message
    /// @param db_error_number
    /// @return
    virtual bool addMessage(std::shared_ptr<Message> &message, uint &db_error_number) = 0;

    /// @brief Получает список публичных сообщений
    /// @param reader_id сообщение будет помечено прочитанным если этот ID не является автором сообщения.
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_error_number
    /// @return
    virtual std::string messageList(ullong reader_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) = 0;

    /// @brief Получает список личных сообщений
    /// @param reader_id сообщение будет помечено прочитанным если этот ID не является автором сообщения.
    /// @param author_id
    /// @param recipient_id
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_error_number
    /// @return
    virtual std::string messageList(ullong reader_id, ullong author_id, ullong recipient_id, ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) = 0;

    /// @brief тестовая функция, указывает какой тип подключения к базе
    virtual void hello() = 0;
};