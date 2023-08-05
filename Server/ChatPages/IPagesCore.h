#pragma once
#include "../../Misc/Misc.h"
#include "../DBClient/DBClient.h"
#include "../ChatCore/User.h"
#include "../../Misc/BufferActions.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

class IPagesCore
{
private:
    char (&cmd_buffer)[CMD_BUFFER];
    DBClient &dbClient;

    std::shared_ptr<User> AuthorizedUser = nullptr;

    std::string page_text;
    std::string cmd_text;
    std::string login;
    ullong session_key;

    BufferActions buffer{cmd_buffer};

    uint pg_start;
    uint pg_per_page;
    uint pg_end;
    sv::pagination pg_mode = sv::pagination::last_page;

public:
    IPagesCore(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient);
    virtual ~IPagesCore() = default;
    virtual void run() = 0;
    virtual bool commandHandler() = 0;
};