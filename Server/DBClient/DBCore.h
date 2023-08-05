#pragma once
#include <string>
#include <memory>
#include <iostream>
#include "../../Misc/Misc.h"
#include "../ChatCore/User.h"
#include "../ChatCore/Message.h"

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
    uint db_errno = 0;

public:
    int provider;
    DBCore();
    virtual ~DBCore() = default;

    /// @brief Инициализация подключения к базе
    virtual bool initialize() = 0;

    /// @brief Получить пользователя из базы по ID
    /// @param userID
    /// @param db_errno
    /// @return
    virtual std::shared_ptr<User> getUserByID(const ullong &userID) = 0;

    /// @brief Получить пользователя по логину
    /// @param userLogin
    /// @param db_errno
    /// @return
    virtual std::shared_ptr<User> getUserByLogin(const std::string &userLogin) = 0;

    /// @brief Получить количество рядов в запросе
    /// @param table
    /// @param where
    /// @param db_errno
    /// @return
    virtual ullong getCount(std::string table, std::string where) = 0;

    /// @brief Вывести список пользователей
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_errno
    /// @return
    virtual std::string userList(ullong &start, ullong &per_page, ullong &capacity) = 0;

    /// @brief Сохранить пользователя если он уже есть в базе
    /// @param user
    /// @param db_errno
    /// @return
    virtual bool saveUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy) = 0;

    /// @brief Добавить нового пользователя в базу
    /// @param user
    /// @param login_busy
    /// @param email_busy
    /// @param db_errno
    /// @return
    virtual bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy) = 0;

    /// @brief Получает сообщение из базы
    /// @param messageID
    /// @param db_errno
    /// @return
    virtual std::shared_ptr<Message> getMessageByID(const ullong &messageID) = 0;

    /// @brief Добавляет новое сообщение в базу
    /// @param message
    /// @param db_errno
    /// @return
    virtual bool addMessage(std::shared_ptr<Message> &message) = 0;

    /// @brief Получает список публичных сообщений
    /// @param reader_id сообщение будет помечено прочитанным если этот ID не является автором сообщения.
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_errno
    /// @return
    virtual std::string messageList(ullong &reader_id, ullong &start, ullong &per_page, ullong &capacity) = 0;

    /// @brief Получает список личных сообщений
    /// @param reader_id сообщение будет помечено прочитанным если этот ID не является автором сообщения.
    /// @param author_id
    /// @param recipient_id
    /// @param start
    /// @param per_page
    /// @param capacity
    /// @param db_errno
    /// @return
    virtual std::string messageList(ullong &reader_id, ullong interlocutor_id, ullong &start, ullong &per_page, ullong &capacity) = 0;

    /// @brief удаление элемента по ID
    /// @param id
    /// @param table
    /// @param db_errno
    /// @return
    virtual bool deleteByID(ullong &id, std::string &table) = 0;

    /// @brief Изменяет битовые флаги статуса
    /// @param id
    /// @param table
    /// @param add
    /// @param db_errno
    /// @return
    virtual bool setStatus(ullong &id, std::string &table, bool add) = 0;

    virtual void DBclose() = 0;

    uint getDBerrno() { return db_errno; }
    void clearDBerrno() { db_errno = 0; }

    /// @brief тестовая функция, указывает какой тип подключения к базе
    virtual void hello() = 0;
};