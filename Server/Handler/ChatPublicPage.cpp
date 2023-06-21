#include "ChatPublicPage.h"

ChatPublicPage::ChatPublicPage(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB,
                               std::string &_page_text,
                               std::string &_cmd_text,
                               std::string &_login,
                               ullong &_session_key,
                               char (&_data_buffer)[DATA_BUFFER],
                               char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _page_text,
                     _cmd_text,
                     _login,
                     _session_key,
                     _data_buffer,
                     _cmd_buffer)
{
    pg_start = buffer.getPgStart();
    pg_per_page = buffer.getPgPerPage();
    pg_end = buffer.getPgEnd();
    pg_mode = buffer.getPaginationMode();
}

void ChatPublicPage::run()
{
    // Проверка авторизации
    authorized_user = usersDB.getUserByLogin(login);
    if ((authorized_user != nullptr && authorized_user->getSessionKey() != session_key && session_key != 0) || authorized_user == nullptr)
    {
        authorized_user = nullptr;
        login = "Guest";
        // если не авторизован, выбрасываем на главную
        buffer.createFlags(sv::clear_console, sv::get_string);
        buffer.writeDynData(login, MAIN_PAGE, NONE);
        Misc::writeStringBuffer("Вы не авторизованы. Введите команду /chat: ", data_buffer);
        return;
    }

    pg_start = buffer.getPgStart();
    pg_per_page = buffer.getPgPerPage();
    pg_mode = buffer.getPaginationMode();

    data_text += "Вы находитесь в публичном чате.\n\n";

    // Обработка команд ввода
    if (page_text == PUBLIC_PAGE_INPUT && commandHandler())
    {
        // commandHandler() либо изменит параметры страницы либо примет решение о выходе
        return;
    }

    if (pg_mode == sv::pagination::last_page)
    {
        // последние 10 сообщений
        auto m = pubMessagesDB.listLast(pg_start, pg_per_page, pg_end);
        data_text += getList(m, "В этом чате нет сообщений. Начните общение первым.", ">>>Сообщение №", pg_start);
    }

    if (pg_mode == sv::pagination::message)
    {
        // список от указанного сообщения
        auto m = pubMessagesDB.list(pg_start, pg_per_page, pg_end);
        data_text += getList(m, "В этом чате нет сообщений. Начните общение первым.", ">>>Сообщение №", pg_start);
    }

    if (!pubMessagesDB.empty())
    {
        data_text += "Показаны сообщения " + std::to_string(pg_start + 1) + " - " + std::to_string(pg_end) + " из " + std::to_string(pubMessagesDB.getCount()) + "\n";
    }
    if (pg_mode == sv::pagination::message && pg_start + pg_per_page < pubMessagesDB.getCount())
    {
        data_text += "Внимание. Вы не увидите последние сообщения потому-что они находятся за диапазоном отображения.\n";
    }

    data_text += "Вы: " + authorized_user->getData();
    data_text += commands_list;

    if (data_text.size() > DATA_BUFFER - 4)
    {
        std::string cutText = "Воспользуйтесь командой /m чтобы прочитать предыдущие сообщения\n\n";
        data_text = Misc::ltrimString(data_text, DATA_BUFFER - 4 - cutText.size());
        data_text = cutText += data_text;
    }

    buffer.createFlags(sv::get_string, sv::clear_console);
    buffer.writeDynData(login, PUBLIC_PAGE_INPUT, cmd_text);
    Misc::writeStringBuffer(data_text, data_buffer);
}

bool ChatPublicPage::commandHandler()
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

        if (pg_start > pubMessagesDB.getCount())
            pg_start = pubMessagesDB.getCount();

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
    if (cmd[0] == "/u")
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
        buffer.writeDynData(login, PRIVATE_PAGE, NONE);
        return true;
    }

    // если не отработала ни одна команда, значит введен текст сообщения

    pubMessagesDB.addMessage(authorized_user->getID(), 0, cmd_text, msg::public_);
    return false;
}
