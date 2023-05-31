#pragma once
#include "../IChatInterface.h"
#include "../Options.h"

class AdminInterface final : public IChatInterface
{
private:
public:
    ~AdminInterface() = default;
    AdminInterface(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB);
    chat::Results run() override;

    /// @brief Главная страница раздела администратора
    /// @return
    chat::Results mainPage();

    /// @brief Страница управления пользователями
    /// @return
    chat::Results usersManage();

    /// @brief Управление блокировкой и правами пользователя
    /// @param description Сообщение при запросе команд
    /// @param status
    /// @return
    chat::Results setUserStatus(std::wstring description, chat::Results status);

    /// @brief Страница управления жалобами
    /// @return
    chat::Results complaintsManage();

    /// @brief Действия с жалобами
    /// @param action
    /// @return
    chat::Results complaintActions(chat::Results action);
};