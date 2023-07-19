#pragma once
#include "SQL.h"
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

class MySQLAPI : public SQL
{
private:
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;

public:
    ~MySQLAPI() = default;

    void initialize() override;

    void hello();
};
