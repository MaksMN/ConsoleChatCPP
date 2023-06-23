#pragma once
#include "../../DB/DBmessages.h"
#include "../../DB/DBusers.h"
#include "../../Misc/Misc.h"
#include "../../DB/DBcomplaints.h"
#include "../../Misc/BufferActions.h"
#include "ServerChatMap.h"
#include <set>

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

class IChatInterface
{
protected:
    DBmessages &pubMessagesDB;
    DBmessages &privMessagesDB;
    DBcomplaints &complaintsDB;
    DBusers &usersDB;

    char (&cmd_buffer)[CMD_BUFFER];

    std::shared_ptr<User> AuthorizedUser = nullptr;

    std::string page_text;
    std::string cmd_text;
    std::string login;
    ullong session_key;

    BufferActions buffer{cmd_buffer};
    ServerChatMap chatMap;

    uint pg_start;
    uint pg_per_page;
    uint pg_end;
    sv::pagination pg_mode = sv::pagination::last_page;

    std::string data_text;

public:
    virtual ~IChatInterface() = default;
    IChatInterface(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB,
                   char (&_cmd_buffer)[CMD_BUFFER]);
    virtual void run() = 0;
    virtual bool commandHandler() = 0;

    template <typename T>
    std::string getList(std::vector<std::shared_ptr<T>> &t, const std::string &&emptyMsg, const std::string &&beforeMsg, const uint &start, bool user_info = true) const;
    std::string getDataText();
    void clearPagination();
};

template <typename T>
inline std::string IChatInterface::getList(std::vector<std::shared_ptr<T>> &t,
                                           const std::string &&emptyMsg,
                                           const std::string &&beforeMsg,
                                           const uint &start,
                                           bool user_info) const
{

    if (t.empty())
    {
        return emptyMsg;
    }
    std::string out;
    uint k = start;
    for (auto &i : t)
    {
        out += beforeMsg + "[" + std::to_string(++k) + "]\n";
        if (user_info)
        {
            auto user = usersDB.getUserByID(i->getOwnerID());
            out += user->getData();
        }
        out += i->getData() + "\n\n";
    }
    return out;
}
