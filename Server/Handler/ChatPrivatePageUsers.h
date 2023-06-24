#pragma once
#include "IChatInterface.h"

class ChatPrivatePageUsers final : public IChatInterface
{
private:
    std::string commands_list =
        "Укажите userid пользователя или команду.\n"
        "Команда: /m:число - перейти к пользователю № в списке (/m:5 - перейти к пользователю №5);\n"
        "Команда: /p:число - количество пользователей на странице (/p:5 - пять пользователей на странице);\n"
        "Команда: /l - режим: всегда последние 10 пользователей;\n"
        "Команда: /chat - перейти в общий чат;\n"
        "Команда: /logout - выйти из чата;\n"
        "Команда: /help - справка;\n"
        "Команда: /u - обновить страницу;\n";
    std::string additional_message;

public:
    ChatPrivatePageUsers(DBmessages &_pubMessagesDB,
                         DBmessages &_privMessagesDB,
                         DBcomplaints &_complaintsDB,
                         DBusers &_usersDB,
                         char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatPrivatePageUsers() = default;

    void run() override;
    bool commandHandler() override;
};
