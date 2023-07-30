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

    void initialize() override;
    std::shared_ptr<User> getUserByID(const ullong &userID, uint &db_error_number) override;
    std::shared_ptr<User> getUserByLogin(const std::string &userLogin, uint &db_error_number) override;
    ullong getCount(std::string table, std::string where, uint &db_error_number) override;
    std::string userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number) override;
    bool saveUser(std::shared_ptr<User> user, uint &db_error_number) override;
    bool addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number) override;

    std::shared_ptr<Message> getMessageByID(const ullong &messageID, uint &db_error_number) override;

    void hello();

private:
    std::shared_ptr<User> fetchUserRow(bool getPassData = true);

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
