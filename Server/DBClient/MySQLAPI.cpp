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

std::shared_ptr<User> MySQLAPI::getUserByID(const ullong &userID, uint &db_error_number)
{
    std::string query = "SELECT * FROM `users` INNER JOIN hash_tab ON `users`.id = `hash_tab`.uid WHERE `id` = '" + std::to_string(userID) + "' LIMIT 1;";
    mysql_query(&mysql, query.data()); // Делаем запрос к таблице

    if (res = mysql_store_result(&mysql))
    {
        row = mysql_fetch_row(res);
        return fetchUserRow();
    }
    else
    {
        db_error_number = mysql_errno(&mysql);
        Misc::printMessage(mysql_error(&mysql));
        mysql_free_result(res);
    }

    return nullptr;
}

std::shared_ptr<User> MySQLAPI::getUserByLogin(const std::string &userLogin, uint &db_error_number)
{
    std::string query = "SELECT * FROM `users` INNER JOIN hash_tab ON `users`.id = `hash_tab`.uid WHERE `login` LIKE '" + userLogin + "' LIMIT 1;";
    mysql_query(&mysql, query.data()); // Делаем запрос к таблице

    // Выводим все что есть в базе через цикл
    if (res = mysql_store_result(&mysql))
    {
        row = mysql_fetch_row(res);
        return fetchUserRow();
    }
    else
    {
        db_error_number = mysql_errno(&mysql);
        Misc::printMessage(mysql_error(&mysql));
        mysql_free_result(res);
    }

    return nullptr;
}

ullong MySQLAPI::getCount(std::string table, std::string where, uint &db_error_number)
{
    db_error_number = 0;
    ullong count = 0;
    std::string query = "SELECT COUNT(*) FROM `" + table + "` WHERE " + where + ";";
    if (querySelect(query, db_error_number) > 0)
    {
        count = atoll(row[0]);
        mysql_free_result(res);
    }
    return count;
}

std::string MySQLAPI::userList(ullong &start, ullong &per_page, ullong &capacity, uint &db_error_number)
{
    db_error_number = 0;
    std::string query;

    ullong count = getCount("users", "`id` != 0", db_error_number);

    Misc::alignPaginator(start, per_page, count);

    query = "SELECT * FROM `users` WHERE `id` != 0 LIMIT " + std::to_string(start - 1) + ", " + std::to_string(start + per_page) + ";";

    capacity = querySelect(query, db_error_number);

    std::string result;
    for (ullong i = 0; i < capacity; i++)
    {
        result += std::to_string(i + 1) + ". "; // порядковый номер
        ullong id = atoll(row[0]);
        std::string login = (std::string)row[1];
        std::string email = (std::string)row[2];
        std::string first_name = (std::string)row[3];
        std::string last_name = (std::string)row[4];
        ullong registered = atoll(row[5]);
        user::status status = (user::status)atoi(row[6]);
        ullong session_key = atoll(row[7]);
        row = mysql_fetch_row(res);
        std::string hash = std::string();
        std::string salt = std::string();
        auto user = std::make_shared<User>(id, login, email, first_name, last_name, registered, status, session_key, hash, salt);
        result += user->userData();
        result += "\n";
    }

    return result;
}

bool MySQLAPI::saveUser(std::shared_ptr<User> user, uint &db_error_number)
{
    std::string query = "UPDATE `users` SET"
                        "`email` = '" +
                        user->getEmail() +
                        "', `first_name` = '" + user->getFirstName() +
                        "', `last_name` = '" + user->getLastName() +
                        "', `registered` = '" + std::to_string(user->getRegistered()) +
                        "', `status` = '" + std::to_string(user->getStatus()) +
                        "', `session_key` = '" + std::to_string(user->getSessionKey()) + "' WHERE `users`.`id` = " + std::to_string(user->getID()) + ";";

    std::string query2 = "UPDATE `hash_tab` SET `hash` = '" + user->getHash() + "', `salt` = '" + user->getSalt() + "' WHERE `hash_tab`.`uid` = " + std::to_string(user->getID()) + ";";

    if (queryUpdate(query, db_error_number) != 0)
    {
        return false;
    }

    return true;
}

bool MySQLAPI::addUser(std::shared_ptr<User> &user, bool &login_busy, bool &email_busy, uint &db_error_number)
{
    db_error_number = 0;
    std::string query;
    query = "SELECT COUNT(*) FROM `users` WHERE `login` LIKE '" + user->getLogin() + "';";
    if (querySelect(query, db_error_number) > 0 && atoi(row[0]) > 0)
    {
        login_busy = true;
        mysql_free_result(res);
        return false;
    }

    query = "SELECT COUNT(*) FROM `users` WHERE `email` LIKE '" + user->getEmail() + "';";
    if (querySelect(query, db_error_number) > 0 && atoi(row[0]) > 0)
    {
        mysql_free_result(res);
        email_busy = true;
        return false;
    }
    mysql_free_result(res);
    query = "INSERT INTO `users` (`id`, `login`, `email`, `first_name`, `last_name`, `registered`, `status`, `session_key`) "
            "VALUES (NULL, '" +
            user->getLogin() +
            "', '" +
            user->getEmail() +
            "', '" +
            user->getFirstName() +
            "', '" +
            user->getLastName() +
            "', '" +
            std::to_string(user->getRegistered()) +
            "', '" +
            std::to_string(user->getStatus()) +
            "', '" +
            std::to_string(user->getSessionKey()) +
            "');";
    if (queryUpdate(query, db_error_number) != 0)
    {
        mysql_free_result(res);
        return false;
    }

    // согласно задания хеши паролей должны быть в отдельной таблице с триггером
    // из за этого приходится у нового пользователя получить автоинкрементный id а потом добавлять хеши
    // если бы хеш хранился в таблице пользователя, можно было бы его засунуть в предыдущем запросе.
    query = "SELECT `id` FROM `users` WHERE `login` LIKE '" + user->getLogin() + "';";
    ullong id;
    if (querySelect(query, db_error_number) > 0)
    {
        id = atoll(row[0]);
        mysql_free_result(res);
    }
    else
    {
        mysql_free_result(res);
        Misc::printMessage("Не удалось получить автоинкрементный ID пользователя");
        return false;
    }

    query = "UPDATE `hash_tab` SET `hash` = '" + user->getHash() + "', `salt` = '" + user->getSalt() + "' WHERE `hash_tab`.`uid` = " + std::to_string(id) + ";";

    if (queryUpdate(query, db_error_number) != 0)
    {
        return false;
    }
    user = getUserByID(id, db_error_number);
    return user != nullptr && db_error_number == 0;
}

void MySQLAPI::hello()
{
    Misc::printMessage("Using MySQL API!");
}

std::shared_ptr<User> MySQLAPI::fetchUserRow()
{
    if (mysql_num_rows(res))
    {
        ullong id = atoll(row[0]);
        std::string login = (std::string)row[1];
        std::string email = (std::string)row[2];
        std::string first_name = (std::string)row[3];
        std::string last_name = (std::string)row[4];
        ullong registered = atoll(row[5]);
        user::status status = (user::status)atoi(row[6]);
        ullong session_key = atoll(row[7]);
        std::string hash = (std::string)row[9];
        std::string salt = (std::string)row[10];
        return std::make_shared<User>(
            id,
            login,
            email,
            first_name,
            last_name,
            registered,
            status,
            session_key,
            hash,
            salt);
        mysql_free_result(res);
    }
    mysql_free_result(res);
    return nullptr;
}

uint MySQLAPI::querySelect(std::string &query, uint &db_error_number)
{
    db_error_number = 0;
    mysql_query(&mysql, query.data());
    if (res = mysql_store_result(&mysql))
    {
        row = mysql_fetch_row(res);
        return mysql_num_rows(res);
    }
    else
    {
        db_error_number = mysql_errno(&mysql);
        Misc::printMessage(mysql_error(&mysql));
        return 0;
    }
    return 0;
}

uint MySQLAPI::queryUpdate(std::string &query, uint &db_error_number)
{
    db_error_number = 0;
    uint query_result = mysql_query(&mysql, query.data());
    if (query_result != 0)
    {
        db_error_number = mysql_errno(&mysql);
        Misc::printMessage(mysql_error(&mysql));
        return query_result;
    }
    return query_result;
}
