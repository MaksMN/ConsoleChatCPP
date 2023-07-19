#pragma once
#include "../../Misc/Misc.h"
#include "MySQLAPI.h"
#include "ODBC.h"
#include <memory>

const extern char config_file[];

class DBClient
{
private:
public:
    SQL *DBprovider;
    MySQLAPI mysqlapi;
    ODBC odbc;
    ~DBClient() = default;
    void initialise();
};
