#include "DBClient.h"

void DBClient::initialise()
{
    if (Misc::getConfigValue(config_file, "GENERAL", "db") == "mysql")
    {
        DBprovider = &mysqlapi;
    }
    else
    {
        DBprovider = &odbc;
    }

    DBprovider->hello();
}
