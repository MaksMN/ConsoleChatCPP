#include "IPagesCore.h"

IPagesCore::IPagesCore(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient)
    : cmd_buffer(_cmd_buffer), dbClient(_dbclient)
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