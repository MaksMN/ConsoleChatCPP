#include "ServerHandler.h"

ServerHandler::ServerHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]) : data_buffer(_data_buffer), cmd_buffer(_cmd_buffer) {}

void ServerHandler::InitialiseDB()
{
    // Сервисный админ должен быь всегда
    if (!std::filesystem::exists(usersDB.getDBfilePath()))
    {
        User admin(0, "admin", "Администратор", "1234", usersDB.getDBfilePath());
        admin.toAdmin();
        admin.writeData();
    }

    usersDB.updateFromFile();
    pubMessagesDB.updateFromFile();
    privMessagesDB.updateFromFile();
}

void ServerHandler::Run()
{
    /*

    Командный пакет
    |Запрос у клиента I/S (1)|clear 0-1 (1)|session_key (8)|login_size(4)|login|page_size(4)|PAGE_TEXT|cmd_size(4)|CMD_TEXT|
    0                        1             2               10                  1                      2
    |  static                                              | dynamic
    Запрос у клиента: I - число S - строка
    Если ожидается ввод числа, записываем его в блок cmd_size

    */

    auto session_key = Misc::getLong(cmd_buffer, 2);

    auto login_size = Misc::getInt(cmd_buffer, 10);
    if (login_size >= CMD_BUFFER)
    {
        badRequest();
        return;
    }

    auto page_pos = Misc::findDynamicData(cmd_buffer, 10, 1);
    if (page_pos == 0)
    {
        badRequest();
        return;
    }

    auto cmd_pos = Misc::findDynamicData(cmd_buffer, 10, 2);
    if (cmd_pos == 0)
    {
        badRequest();
        return;
    }

    auto login = Misc::getString(cmd_buffer, 10);
    auto page_text = Misc::getString(cmd_buffer, page_pos);
    auto cmd_text = Misc::getString(cmd_buffer, cmd_pos);

    // Запишем в буфер данные на случай если при обработке данные не изменятся.
    Misc::writeStringBuffer("Вы ввели неизвестную команду.\nВведите команду: ", data_buffer);

    if (cmd_text == "/hello")
    {
        Misc::writeStringBuffer("Привет, " + login + "! Я сервер, я живой.\nВведите команду: ", data_buffer);
        clearConsole(false);
        return;
    }
    if (cmd_text == "/help")
    {
        std::string str =
            "Команда /chat - перейти на главную страницу из любого раздела;\n"
            "Команда /help - справка;\n"
            "Команда /hello - опрос сервера;\n"
            "Команда /logout - выйти;\n"
            "Команда /quit - закрыть программу;\n"
            "Команды администратора:"
            "Команда /sv_quit - завершить работу сервера;\n";
        Misc::writeStringBuffer(str + "\nВведите команду: ", data_buffer);
        clearConsole(false);
        return;
    }

    if (page_text == "MAIN_PAGE" && cmd_text != "/chat")
        return;
    if (cmd_text == "/chat")
        page_text == "MAIN_PAGE";

    user = usersDB.getUserByLogin(login);
    if ((user != nullptr && user->getSessionKey() != session_key) || user == nullptr)
    {
        user = nullptr;
        login = "Guest";
    }

    if (user == nullptr)
    {
        ChatGuestPage guestPage{pubMessagesDB, privMessagesDB, complaintsDB, usersDB, page_text, cmd_text, login, session_key, data_buffer, cmd_buffer};
        guestPage.run();
        return;
    }

    if (cmd_text.compare("/sv_quit") == 0)
    {
        if (user != nullptr && user->isAdmin())
        {
            quit();
            Misc::writeStringBuffer("Сервер завершил свою работу.\nВведите команду /quit чтобы завершить работу клиента\nили команду chat когда запустите сервер: ", data_buffer);
        }
        else
        {
            Misc::writeStringBuffer("Вы ввели команду доступную только администраторам.\nВведите команду: ", data_buffer);
            clearConsole(false);
        }
        return;
    }

    // Запишем в буфер данные если ни одна из команд не попала под условия обработки.
    Misc::writeStringBuffer("Не найдена страница для вашей команды.\nВведите команду: ", data_buffer);
    clearConsole(false);
    return;
}

void ServerHandler::badRequest()
{
    Misc::writeStringBuffer("Что-то пошло не так. На сервер пришли данные неверной длинны.", data_buffer);
}

bool ServerHandler::getWork()
{
    return work;
}

void ServerHandler::quit()
{
    work = false;
}

void ServerHandler::clearConsole(bool status)
{
    cmd_buffer[1] = status;
}

void ServerHandler::inputClient(char input)
{
    cmd_buffer[0] = input;
}
