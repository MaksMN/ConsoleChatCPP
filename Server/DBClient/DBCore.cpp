#include "DBCore.h"

DBCore::DBCore() : server(Misc::getConfigValue(config_file, "DB", "server")),
                   port(Misc::getConfigValue(config_file, "DB", "port")),
                   dbuser(Misc::getConfigValue(config_file, "DB", "dbuser")),
                   dbpass(Misc::getConfigValue(config_file, "DB", "dbpass")),
                   dbname(Misc::getConfigValue(config_file, "DB", "dbname")),
                   db_character_set(Misc::getConfigValue(config_file, "DB", "db_character_set"))

{
    Misc::printMessage("Im SQL Base Class constructor!");
}

void DBCore::hello()
{
    Misc::printMessage("Hello! Im SQL Base Class!");
}