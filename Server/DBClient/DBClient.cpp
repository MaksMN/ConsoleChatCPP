#include "DBClient.h"

void DBClient::initialise()
{
    if (Misc::getConfigValue(config_file, "GENERAL", "db") == "mysqlapi")
    {
        DBprovider = &mysqlapi;
    }
    else
    {
        DBprovider = &odbc;
    }

    DBprovider->hello();
}
