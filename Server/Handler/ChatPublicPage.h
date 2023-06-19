#pragma once
#include "IChatInterface.h"

class ChatPublicPage final : public IChatInterface
{
private:
    /* data */
public:
    ChatPublicPage(DBmessages &_pubMessagesDB,
                  DBmessages &_privMessagesDB,
                  DBcomplaints &_complaintsDB,
                  DBusers &_usersDB,
                  std::string &page_text,
                  std::string &cmd_text,
                  std::string &login,
                  ullong &session_key,
                  char (&_data_buffer)[DATA_BUFFER],
                  char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatPublicPage() = default;

    void run();
};
