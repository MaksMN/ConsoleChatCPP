#pragma once
#include "DBCore.h"

class MySQLAPI final : public DBCore
{
private:
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;

    uint select_count = 0;
    MYSQL_ROW_OFFSET select_offset;
    void select() { select_count = 0; }

public:
    ~MySQLAPI() = default;

    /// @brief инициализация подключения к базе данных.
    void initialize();
    /// @brief тестовая функция, указывает какой тип подключения к базе
    void hello();

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

    void hello();

private:
    std::shared_ptr<User> fetchUserRow(uint startRow = 0, bool getPassData = true);
    std::shared_ptr<Message> fetchMessageRow(uint startRow = 0, bool pub = true);

    /// @brief Делает запрос SELECT
    /// @param query
    /// @param db_error_number
    /// @return количество найденных рядов
    uint querySelect(std::string &query, uint &db_error_number);

    /// @brief Делает запрос UPDATE
    /// @param query
    /// @param db_error_number
    /// @return число возвращаемое mysql_query
    uint queryUpdate(std::string &query, uint &db_error_number);
};
