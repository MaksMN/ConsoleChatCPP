#pragma once
#include "IChatInterface.h"

class ChatPrivatePageMessages final : public IChatInterface
{
private:
    std::string commands_list =
        "Вы можете ввести текст сообщения или команду.\n"
        "Команда: /m:число - перейти к сообщению №... (/m:5 - перейти к сообщению №5);\n"
        "Команда: /p:число - количество сообщений на странице (/p:5 - пять сообщений на странице);\n"
        "Команда: /l - режим: всегда последние 10 сообщений;\n"
        "Команда: /pm - перейти в личные сообщения;\n"
        "Команда: /chat - перейти в общий чат;\n"
        "Команда: /logout - выйти из чата;\n"
        "Команда: /help - справка;\n"
        "Команда: /u - обновить страницу для получения новых сообщений;\n"
        "Введите текст сообщения или команду: ";

    std::shared_ptr<User> discussant = nullptr;

public:
    ChatPrivatePageMessages(DBmessages &_pubMessagesDB,
                            DBmessages &_privMessagesDB,
                            DBcomplaints &_complaintsDB,
                            DBusers &_usersDB,
                            char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatPrivatePageMessages() = default;

    void run() override;
    bool commandHandler() override;
};
