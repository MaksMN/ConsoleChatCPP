#include "IPagesCore.h"

class UserAuthPage final : public IPagesCore
{
private:
    std::string available_commands =
        "Доступные команды:\n"
        "/auth:логин:пароль - авторизоваться;\n"
        "/reg - зарегистрироваться;\n"
        "Введите команду: ";

public:
    UserAuthPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient);
    ~UserAuthPage() = default;

    void run() override;
    bool commandHandler() override;

private:
    bool authCommand();
    void offerRegisterOrLogin();
};
