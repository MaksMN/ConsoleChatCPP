#pragma once
#include <string>
#include "../../Misc/Misc.h"
#include "../../Misc/Flags.h"
#include "../../Misc/SHA1.h"

typedef unsigned int uint;
typedef unsigned long long ullong;

/// @brief Класс контейнер данных пользователя.
class User
{
private:
    ullong _id = 0;
    std::string _login;
    std::string _name;
    std::string pass_hash;
    std::string pass_salt;
    user::status _status = user::none_;
    ullong _timestamp = 0;

    Flags<user::status> flags;
    ullong session_key = 0;
    SHA1 sha1;

public:
    /// @brief Создает нового пользователя
    /// @param id
    /// @param login
    /// @param name
    /// @param pass
    User(const ullong &id, const std::string &login, const std::string &name, std::string &pass);
    User(const ullong &&id, const std::string &&login, const std::string &&name, std::string &&pass);

    /// @brief Создает пользователя по id из базы данных
    /// @param id
    User(ullong &id);
    User(ullong &&id);

    ~User() = default;

    /// @brief Пересоздает пользователя из базы данных
    /// @param id
    /// @return
    User get(ullong &id);

    /// @brief сохраняет пользователя в БД
    void save();

    /// @brief Возвращает ID пользователя
    /// @return
    uint getID();
    /// @brief Возвращает имя пользователя
    /// @return
    std::string getName();

    /// @brief Изменяет имя пользователя
    /// @param name
    void setName(const std::string &name);

    /// @brief Возвращает логин пользователя
    /// @return
    std::string getLogin();

    /// @brief Получает флаги статуса пользователя
    user::status getStatus();

    /// @brief Меняет флаги статуса пользователя
    void setStatus(user::status &status);

    /// @brief Проверяет флаг блокировки пользователя
    bool isBanned();

    /// @brief Удаляет флаг блокировки пользователя
    /// @return
    void unBan();

    /// @brief Добавляет флаг блокировки пользователя
    void ban();

    /// @brief Проверяет флаг администратора
    bool isAdmin();
    /// @brief Проверяет флаг пользователя
    /// @return
    bool isUser();

    /// @brief Устанавливает флаг администратора
    void toAdmin();

    /// @brief Устанавливает флаг пользователя
    void toUser();

    /// @brief Выводит на экран данные пользователя
    void printData();

    std::string getData();

    /// @brief Изменяет пароль пользователя
    void setPass(std::string &pass);

    /// @brief проверяет пароль пользователя
    bool validatePass(std::string &pass);

    /// @brief Записывает данные пользователя в файл
    void writeData();

    ullong getSessionKey();
    void setSessionKey(ullong key);

    uint getOwnerID();
};
