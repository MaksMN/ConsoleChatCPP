#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include <map>
#include <set>
#include "../../Misc/Misc.h"
#include "../../DB/Headers.h"
#include "IChatInterface.h"
#include "ChatGuestPage.h"
#include "ChatPublicPage.h"
#include "ChatPrivatePageUsers.h"
#include "ChatPrivatePageMessages.h"
#include "ChatEditProfile.h"
#include "../../Misc/BufferActions.h"
#include "ServerChatMap.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

typedef unsigned int uint;
typedef unsigned long long ullong;
class ServerHandler
{
private:
    char (&cmd_buffer)[CMD_BUFFER];

    DBmessages pubMessagesDB{"pub_messages"};
    DBmessages privMessagesDB{"priv_messages"};
    DBcomplaints complaintsDB;
    DBusers usersDB{"users"};

    std::shared_ptr<User> user;

    bool work = true;

    ServerChatMap chatMap;

    std::set<std::string> pages_set;

    std::string data_buffer_text;

    BufferActions buffer{cmd_buffer};

public:
    explicit ServerHandler(char (&_cmd_buffer)[CMD_BUFFER]);
    ~ServerHandler() = default;
    void InitialiseDB();
    void Run();
    void badRequest();

    /// @brief Получает статус работы сервера;
    /// @return
    bool getWork();
    /// @brief дает команду завершить работу сервера
    void quit();

    /// @brief Дает команду клиенту очистить консоль
    /// @param status
    void clearConsole(bool status = true);

    /// @brief задает буферу данные по умолчанию
    void clearBuffer();

    /// @brief Возвращает текст который надо отправить клиенту
    /// @return
    std::string &getDataText();
};