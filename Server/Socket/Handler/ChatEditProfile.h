#pragma once
#include "IChatInterface.h"
class ChatEditProfile final : public IChatInterface
{
private:
    std::string commands_list =
        "Вы находитесь на странице изменения данных профиля;\n"
        "Команда: /name:new_name - изменить имя (/name:Иван изменит имя на Иван);\n"
        "Команда: /pass:new_pass - изменить пароль (/pass:1234 изменит имя на 1234);\n"
        "Команда: /chat - перейти в чат;\n"
        "Команда: /help - справка;\n"
        "Введите команду: ";

public:
    ChatEditProfile(DBmessages &_pubMessagesDB,
                    DBmessages &_privMessagesDB,
                    DBcomplaints &_complaintsDB,
                    DBusers &_usersDB,
                    char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatEditProfile() = default;
    void run() override;
    bool commandHandler() override;
};