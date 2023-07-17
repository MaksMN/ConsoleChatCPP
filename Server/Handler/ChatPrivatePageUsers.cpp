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

    if (AuthorizedUser->isAdmin())
    {
        data_text += "Вы находитесь на странице управления пользователями.\n\n";
    }
    else
    {
        data_text += "Вы находитесь на странице выбора пользователей для личных сообщений.\n\n";
    }

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
        data_text += "Показаны пользователи " + std::to_string(pg_start + 1) + " - " + std::to_string(pg_end) + " из " + std::to_string(usersDB.getCount()) + "\n";
    }
    if (pg_mode == sv::pagination::message && pg_start + pg_per_page < usersDB.getCount())
    {
        data_text += "Внимание. Вы не увидите последних пользователей потому-что они находятся за диапазоном отображения.\n";
    }

    data_text += "Вы: " + AuthorizedUser->getData();

    if (buffer.isNotFoundPmUserID())
        data_text += "\nПользователь для личных сообщений не найден.\n\n";
    data_text += additional_message;
    additional_message.clear();
    data_text += commands_list;
    if (AuthorizedUser->isAdmin())
    {
        data_text += "Команда: /ban:userid - заблокировать пользователя (/ban:5 - заблокировать [userid 5] )\n";
        data_text += "Команда: /unban:userid - разблокировать пользователя (/unban:5 - разблокировать [userid 5] )\n";
    }

    if (AuthorizedUser->isAdmin() && AuthorizedUser->getID() == 0)
    {
        data_text += "Команда: /adm:userid - сделать пользователя администратором (/adm:5 - сделать администратором [userid 5] )\n";
        data_text += "Команда: /unadm:userid - снять с должности администратора (/adm:5 - сделать администратором [userid 5] )\n";
    }

    data_text += "Введите команду или userid чтобы начать с ним беседу: ";

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

    if (cmd[0] == "/ban" || cmd[0] == "/unban" || cmd[0] == "/adm" || cmd[0] == "/unadm")
    {
        if (!AuthorizedUser->isAdmin())
        {
            return false;
        }
        uint userid;
        if (cmd.size() > 1)
        {
            userid = atoi(cmd[1].data());
        }
        else
        {
            return false;
        }

        if (userid == 0)
        {
            buffer.createFlags(sv::get_string);
            data_text = "\nНеприменимо для сервисного администратора\n";
            data_text += "Введите команду или userid чтобы начать с ним беседу: ";
            return true;
        }

        auto user = usersDB.getUserByID(userid);

        if (user == nullptr)
        {
            buffer.createFlags(sv::get_string);
            data_text = "\nВы ввели неверный userid\n";
            data_text += "Введите текст сообщения или команду: ";
            return true;
        }
        if (cmd[0] == "/ban")
        {
            if (user->getID() == AuthorizedUser->getID())
            {
                buffer.createFlags(sv::get_string);
                data_text = "\nВы не можете забанить себя являясь администратором. Обратитесь к другому администратору.\n";
                data_text += "Введите текст сообщения или команду: ";
                return true;
            }
            user->ban();

            usersDB.updateFiles();
            return false;
        }
        else if (cmd[0] == "/unban")
        {
            user->unBan();
            usersDB.updateFiles();
            return false;
        }
        else if (cmd[0] == "/adm" || AuthorizedUser->getID() == 0)
        {
            user->toAdmin();
            usersDB.updateFiles();
            return false;
        }
        else if (cmd[0] == "/unadm" || AuthorizedUser->getID() == 0)
        {
            user->toUser();
            usersDB.updateFiles();
            return false;
        }
        return false;
    }

    // если не отработала ни одна команда, значит введен userid
    clearPagination();
    uint uid = atoi(cmd_text.data());
    buffer.setPmUserID(uid);
    buffer.createFlags(sv::no_input, sv::clear_console);
    buffer.writeDynData(login, PRIVATE_PAGE_MESSAGES, cmd_text);
    return true;
}
