#include "UserAuthPage.h"

UserAuthPage::UserAuthPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient)
    : IPagesCore(_cmd_buffer, _dbclient) {}

void UserAuthPage::run()
{
}

bool UserAuthPage::commandHandler()
{
    return false;
}
