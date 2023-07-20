#pragma once
#include "SQL.h"
#include "ODBC.h"
#include <mysql.h>
#pragma comment(lib, "libmysql.lib")

class MySQLAPI : public SQL
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

    template <typename T, typename... Args>
    void select(T value, Args... args);

    void hello();
};

template <typename T, typename... Args>
inline void MySQLAPI::select(T value, Args... args)
{
    if (select_count == 0)
    {
        mysql_query(&mysql, value);
        res = mysql_store_result(&mysql);
    }

    if (!res)
    {
        Misc::printMessage("Ошибка MySql номер ", false);
        Misc::printMessage(mysql_error(&mysql));
        return;
    }
    else if (select_count == 0)
    {
        select_offset = mysql_row_tell(res);
    }

    if (select_count != 0)
    {
        while (row = mysql_fetch_row(res))
        {
            for (uint i = 0; i < mysql_num_fields(res); i++)
            {
                value.push_back(row[i]);
            }
        }
    }
    mysql_row_seek(res, select_offset);
    select_count++;
    select(args...);
}
