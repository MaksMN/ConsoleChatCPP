#include "UserAuthPage.h"

UserAuthPage::UserAuthPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient)
    : IPagesCore(_cmd_buffer, _dbclient) {}

void UserAuthPage::run()
{
    offerRegisterOrLogin();
}

bool UserAuthPage::commandHandler()
{
    return false;
}

void UserAuthPage::offerRegisterOrLogin()
{
    data_text = "Вы не авторизованы.\n"
                "Доступные команды:\n"
                "/auth:логин:пароль - авторизоваться;\n"
                "/reg - зарегистрироваться;\n"
                "Введите команду: ";
    buffer.addFlags(sv::get_string, sv::clear_console);
}
