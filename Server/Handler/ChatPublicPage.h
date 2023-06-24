#pragma once
#include "IChatInterface.h"

class ChatPublicPage final : public IChatInterface
{
private:
    std::string commands_list =
        "Вы можете ввести текст сообщения или команду.\n"
        "Команда: /m:число - перейти к сообщению № (/m:5 - перейти к сообщению №5);\n"
        "Команда: /p:число - количество сообщений на странице (/p:5 - пять сообщений на странице);\n"
        "Команда: /l - режим: всегда последние 10 сообщений;\n"
        "Команда: /u - обновить страницу для получения новых сообщений;\n"
        "Команда: /logout - выйти из чата;\n"
        "Команда: /help - справка;\n";

public:
    ChatPublicPage(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB,
                   char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatPublicPage() = default;

    void run() override;

    bool commandHandler() override;
};
