#pragma once
#include "../../Misc/Misc.h"
#include "MySQLAPI.h"
#include "ODBC.h"
#include <memory>

const extern char config_file[];

class DBClient
{
private:
    MySQLAPI mysqlapi;
    ODBC odbc;
    DBCore *_DBprovider;

public:
    DBCore *DBprovider();
    ~DBClient() = default;
    void initialise();
};
