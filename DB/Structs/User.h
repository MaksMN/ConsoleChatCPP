#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include "../../Misc/sha1.h"
#include "../../Misc/Flags.h"
#include "../Options.h"
#include "../../Misc/DateTime.h"
#include <fstream>
#include <cstring>
#include "../../Misc/Stream.h"

typedef unsigned int uint;

/// @brief Класс контейнер данных пользователя.
class User
{
private:
    const uint _id = 0;
    const std::string _login;
    std::string _name;
    uint *_pass_hash;
    const int _pass_bytes = 128; // размер сообщения для хеширования, пароль + соль
    char _pass_salt[64];         // соль должна быть в 2 раза меньше сообщения для хеширования
    user::status _status = user::none_;
    const unsigned long long _timestamp; // дата регистрации
    Flags<user::status> flags;
    std::string DBfilePath;

public:
    /// @brief Создает нового пользователя путем указания данных в аргументах
    /// @param id
    /// @param login
    /// @param name
    /// @param pass
    /// @param db_file путь к файлу базы пользователей
    User(const uint &id, const std::string &login, const std::string &name, std::string &pass, std::string db_file);

    /// @brief Создает нового пользователя
    /// @param id
    /// @param login
    /// @param name
    /// @param pass
    /// @param db_file путь к файлу базы пользователей
    User(const uint &&id, const std::string &&login, const std::string &&name, std::string &&pass, std::string db_file);

    /// @brief Создает нового пользователя из файла базы
    /// @param stream
    /// @param db_file путь к файлу базы пользователей
    User(std::ifstream &stream, std::string db_file);
    ~User();

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

    /// @brief Изменяет пароль пользователя
    void setPass(std::string &pass);

    /// @brief проверяет пароль пользователя
    bool validatePass(std::string &pass);

    /// @brief Записывает данные пользователя в файл
    void writeData();

private:
    /// @brief генерирует данные для хеширования
    void bytesForHash(const std::string &pass, char message[]);
    /// @brief Генерирует соль
    void setSalt(const unsigned &seed);
    /// @brief Удаляет данные для хеширования
    void clearHashMessage(char message[]);
};
