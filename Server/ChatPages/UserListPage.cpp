#include "UserListPage.h"

UserListPage::UserListPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient, std::shared_ptr<User> &authorizedUser)
    : IPagesCore(_cmd_buffer, _dbclient, authorizedUser) {}

void UserListPage::run()
{
}
