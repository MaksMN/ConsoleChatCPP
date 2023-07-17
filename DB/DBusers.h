#pragma once
#include "DBcore.h"

/// @brief Класс для работы с базой данных пользователей
class DBusers final : public DBcore<User>
{
private:
public:
    explicit DBusers(const std::string &db_path);
    virtual ~DBusers() = default;
    /// @brief Проверяет уникальность логина
    /// @param login
    /// @return
    bool uniqueLogin(std::string &login) const;

    /// @brief Добавляет пользователя в базу
    /// @param login
    /// @param name
    /// @param pass
    /// @return
    std::shared_ptr<User> addUser(const std::string &login, const std::string &name, std::string &pass);

    /// @brief Добавляет пользователя в базу
    /// @param login
    /// @param name
    /// @param pass
    /// @return
    std::shared_ptr<User> addUser(const std::string &&login, const std::string &&name, std::string &&pass);

    /// @brief Получает указательна пользователя в базе по ID
    /// @param id
    /// @return
    std::shared_ptr<User> getUserByID(uint id);

    /// @brief Ищет пользователя в базе по логину
    /// @param login
    /// @return
    std::shared_ptr<User> getUserByLogin(std::string &login);

    /// @brief Выгружает данные в базу из файла
    void updateFromFile();

    /// @brief возвращает путь к файлу базы
    /// @return
    std::string getDBfilePath();

    /// @brief изменяет путь к файлу базы
    /// @param path
    void setDBfilePath(const std::string &path);
};