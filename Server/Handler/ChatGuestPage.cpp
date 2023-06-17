#include "ChatGuestPage.h"

ChatGuestPage::ChatGuestPage(DBmessages &_pubMessagesDB,
                             DBmessages &_privMessagesDB,
                             DBcomplaints &_complaintsDB,
                             DBusers &_usersDB,
                             char (&_data_buffer)[DATA_BUFFER],
                             char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB, _privMessagesDB, _complaintsDB, _usersDB, data_buffer, _cmd_buffer) {}

void ChatGuestPage::run(std::string &pageText, std::string &cmdText)
{
}
