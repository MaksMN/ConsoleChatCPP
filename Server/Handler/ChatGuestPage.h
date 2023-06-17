#pragma once
#include "IChatInterface.h"
class ChatGuestPage final : public IChatInterface
{
private:
    /* data */
public:
    ChatGuestPage(DBmessages &_pubMessagesDB,
                  DBmessages &_privMessagesDB,
                  DBcomplaints &_complaintsDB,
                  DBusers &_usersDB,
                  char (&_data_buffer)[DATA_BUFFER],
                  char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatGuestPage() = default;

    void run(std::string &pageText, std::string &cmdText);
};
