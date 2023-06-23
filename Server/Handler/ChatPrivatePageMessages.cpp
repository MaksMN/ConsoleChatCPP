#include "ChatPrivatePageMessages.h"

ChatPrivatePageMessages::ChatPrivatePageMessages(DBmessages &_pubMessagesDB, DBmessages &_privMessagesDB, DBcomplaints &_complaintsDB, DBusers &_usersDB, char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _cmd_buffer) {}

void ChatPrivatePageMessages::run()
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

    uint pm_user_id = buffer.getPmUserID();
    discussant = usersDB.getUserByID(pm_user_id);
    if (discussant == nullptr)
    {
        buffer.setPaginationMode(sv::last_page);
        buffer.setPgPerPage(10);
        buffer.setPgStart(1);
        buffer.setPgEnd(1);
        buffer.createFlags(sv::no_input, sv::clear_console);
        buffer.writeDynData(login, PRIVATE_PAGE_USERS, PM);
        buffer.PmUserIDNoFound();
        return;
    }

    pg_start = buffer.getPgStart();
    pg_per_page = buffer.getPgPerPage();
    pg_mode = buffer.getPaginationMode();

    data_text += "Вы находитесь на странице личных сообщений.\n\n";

    // Обработка команд ввода
    if (page_text == PRIVATE_PAGE_MESSAGES_INPUT)
    {
        // commandHandler() либо изменит параметры страницы либо примет решение о выходе
        if (commandHandler())
            return;
    }

    uint all;
    if (pg_mode == sv::pagination::last_page)
    {
        // последние 10 сообщений
        auto m = privMessagesDB.getPrivateMsgList(AuthorizedUser->getID(), discussant->getID(), pg_start, pg_per_page, pg_end, all);
        data_text += getList(m, "В этом чате нет сообщений. Начните общение первым.\n", ">>>Сообщение №", pg_start);
    }

    if (pg_mode == sv::pagination::message)
    {
        // список от указанного сообщения
        auto m = privMessagesDB.getPrivateMsgList(AuthorizedUser->getID(), discussant->getID(), pg_start, pg_per_page, pg_end, all, false);
        data_text += getList(m, "В этом чате нет сообщений. Начните общение первым.\n", ">>>Сообщение №", pg_start);
    }

    if (all > 0)
    {
        data_text += "Показаны сообщения " + std::to_string(pg_start + 1) + " - " + std::to_string(pg_end) + " из " + std::to_string(all) + "\n";
    }
    else
    {
        data_text += "Показаны сообщения 0 из " + std::to_string(all) + "\n";
    }
    if (pg_mode == sv::pagination::message && pg_start + pg_per_page < privMessagesDB.getCount())
    {
        data_text += "Внимание. Вы не увидите последние сообщения потому-что они находятся за диапазоном отображения.\n";
    }

    data_text += "\nВы: " + AuthorizedUser->getData();
    data_text += "Собеседник: " + discussant->getData() + "\n";
    if (AuthorizedUser->getID() == discussant->getID())
        data_text += "Вы ведете беседу с самим собой.\n";

    data_text += commands_list;

    buffer.createFlags(sv::get_string, sv::clear_console);
    buffer.writeDynData(login, PRIVATE_PAGE_MESSAGES_INPUT, cmd_text);
}

bool ChatPrivatePageMessages::commandHandler()
{
    auto cmd = Misc::stringExplode(cmd_text, ":");
    if (cmd.empty())
        return false;

    // выбрать номер сообщения
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
    // количество сообщений на страницу
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
    if (cmd[0] == "/pm")
    {
        buffer.setPaginationMode(sv::last_page);
        buffer.setPgPerPage(10);
        buffer.setPgStart(1);
        buffer.setPgEnd(1);
        buffer.createFlags(sv::no_input, sv::clear_console);
        buffer.writeDynData(login, PRIVATE_PAGE_USERS, PM);
        return true;
    }

    // если не отработала ни одна команда, значит введен текст сообщения

    privMessagesDB.addMessage(AuthorizedUser->getID(), discussant->getID(), cmd_text, msg::public_);
    return false;
}
