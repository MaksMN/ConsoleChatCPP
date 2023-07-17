#pragma once
#include "IChatInterface.h"
class ChatGuestPage final : public IChatInterface
{
private:
    /* data */
public:
    ChatGuestPage(DBmessages &_pubMessagesDB,
                  DBmessages &_privMessagesDB,
                  DBcomplaints &_complaintsDB,
                  DBusers &_usersDB,
                  char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatGuestPage() = default;

    void run() override;

    /// @brief Страница с предложением авторизации или регистрации
    /// @param message Дополнительное сообщение от функций валидации.
    void offerRegisterOrLogin(const std::string &message = std::string());

    /// @brief Страница авторизации
    /// @param message Дополнительное сообщение от функций валидации.
    void loginPage(const std::string &message = std::string());
    void validateLogin();

    /// @brief Страница регистрации
    /// @param message Дополнительное сообщение от функций валидации.
    void registrationPage(const std::string &message = std::string());
    void validateRegistration();
    bool commandHandler() override { return false; };
};
