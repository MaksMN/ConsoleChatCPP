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

    mysql_query(&mysql, "SELECT * FROM `users`"); // Делаем запрос к таблице

    // Выводим все что есть в базе через цикл
    if (res = mysql_store_result(&mysql))
    {
        while (row = mysql_fetch_row(res))
        {
            for (uint i = 0; i < mysql_num_fields(res); i++)
            {
                Misc::printMessage(row[i]);
            }
        }
    }
    else
    {
        Misc::printMessage("Ошибка MySql номер ", false);
        Misc::printMessage(mysql_error(&mysql));
    }
}

void MySQLAPI::hello()
{
    Misc::printMessage("Using MySQL API!");
}