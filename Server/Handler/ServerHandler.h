#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include "../../Misc/Misc.h"
#include "../../DB/Headers.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

typedef unsigned int uint;
typedef unsigned long long ullong;
class ServerHandler
{
private:
    char (&data_buffer)[DATA_BUFFER];
    char (&cmd_buffer)[CMD_BUFFER];

    std::string pageText;
    uint pagePos;
    uint pageSize;

    std::string cmdText;
    uint cmdPos;
    uint cmdSize;

    ullong session_key;
    uint login_size;
    std::string login;

    DBmessages pubMessagesDB{"pub_messages"};
    DBmessages privMessagesDB{"priv_messages"};
    DBcomplaints complaintsDB;
    DBusers usersDB{"users"};

public:
    ServerHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]);
    ~ServerHandler() = default;
    void InitialiseDB();
    void Run();
    void badRequest();
};