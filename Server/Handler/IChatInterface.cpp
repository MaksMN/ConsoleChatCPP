#include "IChatInterface.h"

IChatInterface::IChatInterface(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB,
                               char (&_data_buffer)[DATA_BUFFER],
                               char (&_cmd_buffer)[CMD_BUFFER])
    : pubMessagesDB(_pubMessagesDB),
      privMessagesDB(_privMessagesDB),
      complaintsDB(_complaintsDB),
      usersDB(_usersDB),
      data_buffer(_data_buffer),
      cmd_buffer(_cmd_buffer) {}