#pragma once
#include "DBCore.h"

class MySQLAPI : public DBCore
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
    std::shared_ptr<User> getUserByID(ullong userID);
    std::shared_ptr<User> getUserByLogin(std::string userLogin);

    void hello();
};
