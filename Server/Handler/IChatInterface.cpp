#include "IChatInterface.h"

IChatInterface::IChatInterface(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB,
                               std::string &_page_text,
                               std::string &_cmd_text,
                               std::string &_login,
                               ullong &_session_key,
                               char (&_data_buffer)[DATA_BUFFER],
                               char (&_cmd_buffer)[CMD_BUFFER])
    : pubMessagesDB(_pubMessagesDB),
      privMessagesDB(_privMessagesDB),
      complaintsDB(_complaintsDB),
      usersDB(_usersDB),
      page_text(_page_text),
      cmd_text(_cmd_text),
      login(_login),
      session_key(_session_key),
      data_buffer(_data_buffer),
      cmd_buffer(_cmd_buffer) {}
