#include "ChatPublicPage.h"

ChatPublicPage::ChatPublicPage(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB,
                               std::string &_page_text,
                               std::string &_cmd_text,
                               std::string &_login,
                               ullong &_session_key,
                               char (&_data_buffer)[DATA_BUFFER],
                               char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _page_text,
                     _cmd_text,
                     _login,
                     _session_key,
                     _data_buffer,
                     _cmd_buffer) {}

void ChatPublicPage::run()
{
}
