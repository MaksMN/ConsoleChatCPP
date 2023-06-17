#pragma once
#include "../../DB/DBmessages.h"
#include "../../DB/DBusers.h"
#include "../../Misc/Misc.h"
#include "../../DB/DBcomplaints.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

class IChatInterface
{
protected:
    DBmessages &pubMessagesDB;
    DBmessages &privMessagesDB;
    DBcomplaints &complaintsDB;
    DBusers &usersDB;

    char (&data_buffer)[DATA_BUFFER];
    char (&cmd_buffer)[CMD_BUFFER];

    std::shared_ptr<User> AuthorizedUser = nullptr;

public:
    virtual ~IChatInterface() = default;
    IChatInterface(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB,
                   char (&_data_buffer)[DATA_BUFFER],
                   char (&_cmd_buffer)[CMD_BUFFER]);
    virtual void run(std::string &pageText, std::string &cmdText) = 0;
};