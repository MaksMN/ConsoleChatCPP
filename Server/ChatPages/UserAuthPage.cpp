#include "UserAuthPage.h"

UserAuthPage::UserAuthPage(char (&_cmd_buffer)[CMD_BUFFER], DBClient &_dbclient)
    : IPagesCore(_cmd_buffer, _dbclient) {}

void UserAuthPage::run()
{
    // если пришла команда auth
    if (authCommand())
    {
        return;
    }

    if (commands[0] == "/reg")
    {
    }
    offerRegisterOrLogin();
}

bool UserAuthPage::commandHandler()
{
    return false;
}

bool UserAuthPage::authCommand()
{
    if (commands[0] == "/auth")
    {
        // если пришла команда /auth и 2 параметра это логин и пароль
        if (commands.size() == 3)
        {
            auto u = dbClient.DBprovider()->getUserByLogin(commands[1]);
            if (u == nullptr)
            {
                // если пользователя нет - отправляем сообщение клиенту.
                data_text = "Пользователь с таким логином не существует.\n" + available_commands;
                buffer.addFlags(sv::get_string, sv::clear_console);
                return true;
            }
            else
            {
                if (u->validatePass(commands[2]))
                {
                    session_key = Misc::getRandomKey();
                    buffer.removeFlag(sv::get_string);
                    buffer.addFlags(sv::no_input, sv::clear_console, sv::write_session);
                    buffer.setSessionKey(session_key);
                    buffer.writeDynData(u->getLogin(), "/chat", "/chat"); // редирект в общий чат
                    data_text = " ";
                    return true;
                }
                else
                {
                    data_text = "Неверный пароль.\n" + available_commands;
                    buffer.addFlags(sv::get_string, sv::clear_console);
                    return true;
                }
            }
        }
    }

    return false;
}

void UserAuthPage::offerRegisterOrLogin()
{
    data_text = "Вы не авторизованы.\n" + available_commands;
    buffer.addFlags(sv::get_string, sv::clear_console);
}
