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

    std::string &page_text;
    std::string &cmd_text;
    std::string &login;
    ullong &session_key;

public:
    virtual ~IChatInterface() = default;
    IChatInterface(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB,
                   std::string &_page_text,
                   std::string &_cmd_text,
                   std::string &_login,
                   ullong &_session_key,
                   char (&_data_buffer)[DATA_BUFFER],
                   char (&_cmd_buffer)[CMD_BUFFER]);
    virtual void run() = 0;

    void writeBuffer();

    /// @brief Дает команду клиенту очистить консоль
    /// @param status
    void clearConsole(bool status = true);

    /// @brief Тип ввода на клиенте
    /// @param input I - число; S - строка;
    void inputClient(char input = 'S');
};