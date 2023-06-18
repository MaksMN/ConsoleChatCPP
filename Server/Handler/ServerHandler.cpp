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
    auto session_key = buffer.getSessionKey();

    auto login_size = buffer.getDynDataSize(LOGIN_COUNT);
    if (login_size >= CMD_BUFFER || login_size == 0)
    {
        badRequest();
        return;
    }

    auto page_size = buffer.getDynDataSize(PAGE_TEXT_COUNT);
    if (page_size == 0 || page_size >= CMD_BUFFER)
    {
        badRequest();
        return;
    }

    auto cmd_size = buffer.getDynDataSize(CMD_TEXT_COUNT);
    if (cmd_size == 0 || cmd_size >= CMD_BUFFER)
    {
        badRequest();
        return;
    }

    auto login = buffer.getDynData(LOGIN_COUNT);
    auto page_text = Misc::getString(cmd_buffer, page_pos);
    auto cmd_text = Misc::getString(cmd_buffer, cmd_pos);

    // Запишем в буфер данные на случай если при обработке данные не изменятся.
    Misc::writeStringBuffer("Вы ввели неизвестную команду.\nВведите команду: ", data_buffer);

    /* Общие команды чата, которые срабатывают в любом месте */
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

    // это позволяет игнорировать произвольный ввод и реагировать только на команды
    if (page_text == "MAIN_PAGE" && cmd_text != "/chat")
        return;
    if (cmd_text == "/chat")
    {
        page_text == "MAIN_PAGE";
        writeBuffer(login, page_text, cmd_text);
    }

    /* Авторизация и регистрация */
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

    /* Команды чата которые срабатывают если пользователь авторизован */

    // закрыть сервер (только админ)
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

    // главная страница чата
    if (page_text == "MAIN_PAGE" && cmd_text == "/chat")
    {
        Misc::writeStringBuffer("Это типа главная страница авторизованного пользователя. ", data_buffer);
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
    if (status)
    {
        cmd_buffer[0] = netOptions.add(cmd_buffer[0], sv::clear_console);
    }
    else
    {
        cmd_buffer[0] = netOptions.remove(cmd_buffer[0], sv::clear_console);
    }
}

void ServerHandler::writeBuffer(std::string &login, std::string &page_text, std::string &cmd_text)
{
    // Динамические данные
    Misc::writeStringBuffer(login, cmd_buffer, 10);
    Misc::writeStringBuffer(page_text, cmd_buffer, Misc::findDynamicData(cmd_buffer, 10, 1));
    Misc::writeStringBuffer(cmd_text, cmd_buffer, Misc::findDynamicData(cmd_buffer, 10, 2));
}