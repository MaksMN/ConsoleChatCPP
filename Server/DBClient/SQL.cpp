#include "SQL.h"

SQL::SQL() : server(Misc::getConfigValue(config_file, "DB", "server")),
             port(Misc::getConfigValue(config_file, "DB", "port")),
             dbuser(Misc::getConfigValue(config_file, "DB", "dbuser")),
             dbpass(Misc::getConfigValue(config_file, "DB", "dbpass")),
             odbcdriver(Misc::getConfigValue(config_file, "DB", "odbcdriver"))
{
    Misc::printMessage("Im SQL Base Class constructor!");
}

void SQL::hello()
{
    Misc::printMessage("Hello! Im SQL Base Class!");
}