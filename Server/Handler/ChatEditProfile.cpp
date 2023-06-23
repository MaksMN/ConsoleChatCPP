#include "ChatEditProfile.h"

ChatEditProfile::ChatEditProfile(
    DBmessages &_pubMessagesDB, DBmessages &_privMessagesDB, DBcomplaints &_complaintsDB, DBusers &_usersDB, char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _cmd_buffer) {}

void ChatEditProfile::run()
{
    // Проверка авторизации
    AuthorizedUser = usersDB.getUserByLogin(login);
    if ((AuthorizedUser != nullptr && AuthorizedUser->getSessionKey() != session_key && session_key != 0) || AuthorizedUser == nullptr)
    {
        AuthorizedUser = nullptr;
        login = "Guest";
        // если не авторизован, выбрасываем на главную
        buffer.createFlags(sv::clear_console, sv::get_string);
        buffer.writeDynData(login, MAIN_PAGE, NONE);
        data_text = "Вы не авторизованы. Введите команду /chat: ";
        return;
    }

    // Обработка команд ввода
    if (page_text == PROFILE_PAGE_INPUT)
    {
        // commandHandler() либо изменит параметры страницы либо примет решение о выходе
        if (commandHandler())
            return;
    }

    pg_start = buffer.getPgStart();
    pg_per_page = buffer.getPgPerPage();
    pg_mode = buffer.getPaginationMode();

    data_text += "Вы находитесь на странице редактирования профиля.\n\n";
    data_text += "Ваши данные: ";
    data_text += AuthorizedUser->getData();
    data_text += commands_list;

    buffer.createFlags(sv::get_string, sv::clear_console);
    buffer.writeDynData(login, PROFILE_PAGE_INPUT, cmd_text);
}

bool ChatEditProfile::commandHandler()
{
    auto cmd = Misc::stringExplode(cmd_text, ":");
    if (cmd.empty())
        return false;

    if (cmd[0] == "/name" || cmd[0] == "/pass")
    {
        std::string value;
        if (cmd.size() > 1)
        {
            value = cmd[1];
        }
        else
        {
            return false;
        }

        if (cmd[0] == "/name")
        {
            AuthorizedUser->setName(value);
            usersDB.updateFiles();
            data_text = "Вы сменили имя\n\n" + data_text;
        }
        if (cmd[0] == "/pass")
        {
            AuthorizedUser->setPass(value);
            usersDB.updateFiles();
            data_text = "Вы сменили пароль\n\n" + data_text;
        }
    }
    return false;
}
