#pragma once
#include "../IChatInterface.h"
#include "../Options.h"

/// @brief Класс интерфейса публичного чата
class UserInterface final : public IChatInterface
{
private:
public:
    ~UserInterface() = default;
    UserInterface(DBmessages &_pubMessagesDB,
                  DBmessages &_privMessagesDB,
                  DBcomplaints &_complaintsDB,
                  DBusers &_usersDB);

    chat::Results run() override;

    /// @brief Страница регистрации
    /// @return
    chat::Results registerPage();

    /// @brief страница входа в чат
    /// @return
    chat::Results loginPage();

    /// @brief Главная страница публичного чата
    /// @return
    chat::Results publicChat();

    /// @brief Главная страница личных сообщений. Выбор пользователя для личной беседы.
    /// @return
    chat::Results privateChat();

    /// @brief Страница личной беседы с пользователем
    /// @param discussant
    /// @return
    chat::Results privateConversation(std::shared_ptr<User> discussant);

    /// @brief Добавляет сообщение в базу публичных сообщений
    /// @return
    chat::Results sendPublicMessage();

    /// @brief Добавляет сообщение в базу личных сообщений
    /// @param discussant
    /// @return
    chat::Results sendPrivateMessage(std::shared_ptr<User> discussant);

    /// @brief страница управления жалобами
    /// @return
    chat::Results complaint();

    /// @brief Интерактивный поиск пользователя
    /// @return
    std::shared_ptr<User> searchUserByID();

    /// @brief Страница управления своим профилем. Смена имени и пароля.
    /// @return
    chat::Results userProfile();
};