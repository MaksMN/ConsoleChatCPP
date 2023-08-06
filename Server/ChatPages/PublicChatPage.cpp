#include "PublicChatPage.h"

PublicChatPage::PublicChatPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient)
    : IPagesCore(_cmd_buffer, _dbclient)
{
    if (cmd_text == "/chat" && cmd_text != page_text)
    {
        buffer.pgClear();
    }
    if (cmd_text == "/pgclear")
    {
        buffer.pgClear();
    }
}

void PublicChatPage::run()
{
}

bool PublicChatPage::commandHandler()
{
    return false;
}
