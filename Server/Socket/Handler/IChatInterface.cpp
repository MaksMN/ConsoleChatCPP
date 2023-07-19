#include "IChatInterface.h"

IChatInterface::IChatInterface(DBmessages &_pubMessagesDB, DBmessages &_privMessagesDB, DBcomplaints &_complaintsDB, DBusers &_usersDB, char (&_cmd_buffer)[CMD_BUFFER])
    : pubMessagesDB(_pubMessagesDB), privMessagesDB(_privMessagesDB), complaintsDB(_complaintsDB), usersDB(_usersDB), cmd_buffer(_cmd_buffer)
{
  login = buffer.getDynDataS(LOGIN_COUNT);
  session_key = buffer.getSessionKey();
  page_text = buffer.getDynDataS(PAGE_TEXT_COUNT);
  cmd_text = buffer.getDynDataS(CMD_TEXT_COUNT);
  pg_start = buffer.getPgStart();
  pg_per_page = buffer.getPgPerPage();
  pg_end = buffer.getPgEnd();
  pg_mode = buffer.getPaginationMode();
}

std::string IChatInterface::getDataText()
{
  return data_text;
}

void IChatInterface::clearPagination()
{
  buffer.setPaginationMode(sv::last_page);
  buffer.setPgPerPage(10);
  buffer.setPgStart(1);
  buffer.setPgEnd(1);
}
