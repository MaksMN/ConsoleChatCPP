#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include "../../Misc/Misc.h"
#include "../../DB/Headers.h"
#include "IChatInterface.h"
#include "ChatGuestPage.h"
#include "../../Misc/BufferActions.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

typedef unsigned int uint;
typedef unsigned long long ullong;
class ServerHandler
{
private:
    char (&data_buffer)[DATA_BUFFER];
    char (&cmd_buffer)[CMD_BUFFER];

    DBmessages pubMessagesDB{"pub_messages"};
    DBmessages privMessagesDB{"priv_messages"};
    DBcomplaints complaintsDB;
    DBusers usersDB{"users"};

    std::shared_ptr<User> user;

    bool work = true;
    BufferActions buffer{cmd_buffer};

public:
    ServerHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]);
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
};