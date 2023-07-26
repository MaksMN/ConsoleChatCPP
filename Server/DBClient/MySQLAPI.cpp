#include "MySQLAPI.h"

void MySQLAPI::initialize()
{
    // Получаем дескриптор соединения
    mysql_init(&mysql);
    if (&mysql == nullptr)
    {
        // Если дескриптор не получен — выводим сообщение об ошибке
        Misc::printMessage("Error: can't create MySQL-descriptor");
    }

    // Подключаемся к серверу
    if (!mysql_real_connect(&mysql, server.data(), dbuser.data(), dbpass.data(), dbname.data(), atoi(port.data()), NULL, 0))
    {
        // Если нет возможности установить соединение с БД выводим сообщение об ошибке
        Misc::printMessage("Error: can't connect to database ", false);
        Misc::printMessage(mysql_error(&mysql));
    }
    else
    {
        // Если соединение успешно установлено выводим фразу — "Success!"
        Misc::printMessage("MySQL connection Success!");
    }
    mysql_set_character_set(&mysql, "utf8mb4");
    // Смотрим изменилась ли кодировка на нужную, по умолчанию идёт latin1
    Misc::printMessage("connection characterset: ", false);
    Misc::printMessage(mysql_character_set_name(&mysql));
}

std::shared_ptr<User> MySQLAPI::getUserByID(ullong userID)
{
    return std::shared_ptr<User>();
}

std::shared_ptr<User> MySQLAPI::getUserByLogin(std::string userLogin)
{
    return std::shared_ptr<User>();
}

void MySQLAPI::hello()
{
    Misc::printMessage("Using MySQL API!");
}