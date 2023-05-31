#pragma once
#include "IDBcore.h"

/// @brief Класс для работы с базой данных пользователей
class DBusers final : public IDBcore<User>
{
private:
    uint lastUserID = 0;

public:
    virtual ~DBusers() = default;
    /// @brief Проверяет уникальность логина
    /// @param login
    /// @return
    bool uniqueLogin(std::wstring &login) const;

    /// @brief Добавляет пользователя в базу
    /// @param login
    /// @param name
    /// @param pass
    /// @return
    std::shared_ptr<User> addUser(const std::wstring &login, const std::wstring &name, std::wstring &pass);

    /// @brief Добавляет пользователя в базу
    /// @param login
    /// @param name
    /// @param pass
    /// @return
    std::shared_ptr<User> addUser(const std::wstring &&login, const std::wstring &&name, std::wstring &&pass);

    /// @brief Получает указательна пользователя в базе по ID
    /// @param id
    /// @return
    std::shared_ptr<User> getUserByID(uint id);

    /// @brief Ищет пользователя в базе по логину
    /// @param login
    /// @return
    std::shared_ptr<User> getUserByLogin(std::wstring &login);
};