#pragma once
#include "IChatInterface.h"

class ChatPublicPage final : public IChatInterface
{
private:
    /* data */
    std::shared_ptr<User> authorized_user = nullptr;

    std::string commands_list =
        "Вы можете ввести текст сообщения или команду.\n"
        "Команды могут разделяться двоеточием. (/m:5 - перейти к сообщению №5)\n"
        "Команда: /m:номер - перейти к сообщению;\n"
        "Команда: /p:номер - количество сообщений на странице;\n"
        "Команда: /l - режим: всегда последние 10 сообщений;\n"
        "Команда: /pm - перейти в личные сообщения;\n"
        "Команда: /help - справка;\n"
        "Команда: /u - обновить страницу;\n"
        "Введите текст сообщения или команду: ";

public:
    ChatPublicPage(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB,
                   char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatPublicPage() = default;

    void run();

    bool commandHandler();
};
