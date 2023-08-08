#pragma once
#include "IPagesCore.h"
class PrivateChatPage final : public IPagesCore
{
private:
    std::shared_ptr<User> interlocutorUser = nullptr;
    std::string caption = "Страница личных сообщений";

public:
    PrivateChatPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient, std::shared_ptr<User> &authorizedUser);
    ~PrivateChatPage() = default;
    void run() override;

    void privateChat();
};
