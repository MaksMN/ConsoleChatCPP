#include "ChatPrivatePageUsers.h"

ChatPrivatePageUsers::ChatPrivatePageUsers(
    DBmessages &_pubMessagesDB,
    DBmessages &_privMessagesDB,
    DBcomplaints &_complaintsDB,
    DBusers &_usersDB,
    char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _cmd_buffer) {}

void ChatPrivatePageUsers::run()
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

    pg_start = buffer.getPgStart();
    pg_per_page = buffer.getPgPerPage();
    pg_mode = buffer.getPaginationMode();

    data_text += "Вы находитесь на странице личных сообщений.\n\n";

    // Обработка команд ввода
    if (page_text == PRIVATE_PAGE_USERS_INPUT && commandHandler())
    {
        // commandHandler() либо изменит параметры страницы либо примет решение о выходе
        return;
    }

    if (pg_mode == sv::pagination::last_page)
    {
        // последние 10 сообщений
        auto m = usersDB.listLast(pg_start, pg_per_page, pg_end);
        data_text += getList(m, "В этом чате нет пользователей.", ">>>Пользователь №", pg_start, false);
    }

    if (pg_mode == sv::pagination::message)
    {
        // список от указанного сообщения
        auto m = usersDB.list(pg_start, pg_per_page, pg_end);
        data_text += getList(m, "В этом чате нет пользователей.", ">>>Пользователь №", pg_start, false);
    }

    if (!privMessagesDB.empty())
    {
        data_text += "Показаны пользователи " + std::to_string(pg_start + 1) + " - " + std::to_string(pg_end) + " из " + std::to_string(privMessagesDB.getCount()) + "\n";
    }
    if (pg_mode == sv::pagination::message && pg_start + pg_per_page < usersDB.getCount())
    {
        data_text += "Внимание. Вы не увидите последних пользователей потому-что они находятся за диапазоном отображения.\n";
    }

    data_text += "Вы: " + AuthorizedUser->getData();

    if (buffer.isNotFoundPmUserID())
        data_text += "\nПользователь для личных сообщений не найден.\n\n";
    data_text += commands_list;

    buffer.createFlags(sv::get_string, sv::clear_console);
    buffer.writeDynData(login, PRIVATE_PAGE_USERS_INPUT, cmd_text);
}

bool ChatPrivatePageUsers::commandHandler()
{
    auto cmd = Misc::stringExplode(cmd_text, ":");
    if (cmd.empty())
        return false;

    // выбрать номер элемента
    if (cmd[0] == "/m")
    {
        if (cmd.size() > 1)
        {
            pg_start = atoi(cmd[1].data());
        }
        else
        {
            pg_start = 1;
        }
        if (pg_start == 0)
            pg_start = 1;

        if (pg_start > privMessagesDB.getCount())
            pg_start = privMessagesDB.getCount();

        pg_start -= 1;
        pg_mode = sv::message;
        buffer.setPaginationMode(pg_mode);
        buffer.setPgStart(pg_start);
        return false;
    }
    // количество элементов на страницу
    if (cmd[0] == "/p")
    {
        if (cmd.size() > 1)
        {
            pg_per_page = atoi(cmd[1].data());
        }
        else
        {
            pg_per_page = 1;
        }
        if (pg_per_page < 1)
            pg_per_page = 1;
        buffer.setPgPerPage(pg_per_page);
        return false;
    }
    if (cmd[0] == "/l")
    {
        pg_start = 1;
        pg_per_page = 10;
        pg_end = 0;
        pg_mode = sv::pagination::last_page;
        buffer.setPaginationMode(sv::last_page);
        buffer.setPgPerPage(10);
        buffer.setPgStart(1);
        buffer.setPgEnd(1);
        return false;
    }
    if (cmd[0] == "/u" || cmd[0] == "/update")
    {
        return false;
    }

    // если не отработала ни одна команда, значит введен userid
    clearPagination();
    buffer.createFlags(sv::no_input, sv::clear_console);
    buffer.writeDynData(login, PRIVATE_PAGE_MESSAGES, cmd_text);
    return true;
}
