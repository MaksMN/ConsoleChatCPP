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
                  std::string &page_text,
                  std::string &cmd_text,
                  std::string &login,
                  ullong &session_key,
                  char (&_data_buffer)[DATA_BUFFER],
                  char (&_cmd_buffer)[CMD_BUFFER]);
    ~ChatGuestPage() = default;

    void run();

    void offerRegisterOrLogin(std::string message = std::string());

    void loginPage(std::string message = std::string());

    void validateLogin();

    void validateLoginFail();

    void validateLoginSuccess();
};
