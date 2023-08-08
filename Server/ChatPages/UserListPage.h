#pragma once

#include "IPagesCore.h"

class UserListPage final : public IPagesCore
{
private:
    /* data */
public:
    UserListPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient, std::shared_ptr<User> &authorizedUser);
    ~UserListPage() = default;

    void run() override;
};
