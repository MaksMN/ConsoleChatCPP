#include "ServerHandler.h"

ServerHandler::ServerHandler(char (&_cmd_buffer)[CMD_BUFFER]) : cmd_buffer(_cmd_buffer)
{
    dbClient.initialise();
}

void ServerHandler::Run()
{
    /* проверка данных буфера */
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

    /* получение данных из буфера */
    auto login = buffer.getDynDataS(LOGIN_COUNT);
    auto page_text = buffer.getDynDataS(PAGE_TEXT_COUNT);
    auto cmd_text = buffer.getDynDataS(CMD_TEXT_COUNT);
    auto session_key = buffer.getSessionKey();

    /*
    Концепция команд
    * /page:cmd_text - принудительно перейти на страницу cmd_text (page_text = cmd_text), по умолчанию /page:chat
    * ввод команды cmd_text без /page - выполнить команду cmd_text на текущей странице page_text
    * плюс могут быть команды общего назначения типа /help /hello итп
    */

    // данные пользователя

    uint db_errno = 0;
    dbClient.DBprovider()->initialize();
    user = dbClient.DBprovider()->getUserByLogin(login, db_errno);
    dbClient.DBprovider()->DBclose();
    if (db_errno)
    {
        onDBerror();
        return;
    }

    if ((user != nullptr && user->validateSessionKey(session_key)) || user == nullptr)
    {
        user = nullptr;
        login = "Guest";
    }

    // Запишем в буфер данные на случай если при обработке данные не изменятся.
    data_buffer_text = "Вы ввели неизвестную команду.\nВведите команду /chat: ";
    // изменим флаги чтобы при неизвестной команде клиент встал с запросом
    buffer.createFlags(sv::get_string);

    /* Команды доступные всем */

    if (cmd_text == "/hello")
    {
        data_buffer_text = "Привет, " + login + "! Я сервер, я живой.\nВведите команду: ";
        clearConsole(false);
        return;
    }
    if (cmd_text == "/help")
    {
        std::string admins_commands = user != nullptr && user->isAdmin()
                                          ? "Команда /sv_quit - (admin)завершить работу сервера;\n"
                                          : std::string();
        data_buffer_text =
            "\nКоманды, которые можно вызвать в любое время:\n"
            "Команда /page:chat - Общий чат;\n"
            "Команда /page:private - Личные сообщения;\n"
            "Команда /page:users - Список пользователей;\n"
            "Команда /hello - опрос сервера;\n"
            "Команда /help - справка;\n"
            "Команда /quit - закрыть клиент;\n"
            "\n"
            "Команды, которые можно вызвать в любое время авторизованным пользователям:\n"
            "Команда /page:profile - сменить имя или пароль;\n"
            "Команда /logout - выйти из чата;\n" +
            admins_commands +
            "\nВведите команду: ";
        clearConsole(false);
        return;
    }

    /* Конец зоны доступных всем команд. */

    /* Авторизация и регистрация */
    /* Сюда попадает любой неавторизованный пользователь */
    if (user == nullptr)
    {
        data_buffer_text = "Guest page";
        return;
    }

    /* Команды чата которые срабатывают если пользователь авторизован */

    // закрыть сервер (только админ)
    if (cmd_text.compare("/sv_quit") == 0)
    {
        if (user != nullptr && user->isAdmin())
        {
            quit();
            data_buffer_text = "Сервер завершил свою работу.\nВведите команду /quit чтобы завершить работу клиента\nили команду chat когда запустите сервер: ";
        }
        else
        {
            data_buffer_text = "Вы ввели команду доступную только администраторам.\nВведите команду: ";
            clearConsole(false);
        }
        return;
    }

    // Выйти
    if (cmd_text == "/logout")
    {
        clearBuffer();
        clearConsole(true);
        data_buffer_text = "Вы вышли из чата. Введите команду /chat: ";
        return;
    }

    if (user->isBanned())
    {
        clearConsole(true);
        data_buffer_text = "Вы заблокированы. Введите команду /logout: ";
        buffer.writeDynData(login, USER_BANNED, NONE);
        return;
    }

    // профиль пользователя

    if (cmd_text == "/profile")
    {
        data_buffer_text = "edit profile page";
        return;
    }

    // главная страница чата

    if (cmd_text == "/chat")
    {
        data_buffer_text = "chat public main page";
        return;
    }

    // Список пользователей
    if (cmd_text == "/users")
    {
        data_buffer_text = "users list";
        return;
    }

    // Сообщения в приватном чате
    if (cmd_text == "/private")
    {
        data_buffer_text = "private chat page";
        return;
    }

    // Запишем в буфер данные если ни одна из команд не попала под условия обработки.
    data_buffer_text = "Не найдена страница для вашей команды.\nВведите команду /chat: ";
    buffer.createFlags(sv::get_string);
    clearConsole(false);
    return;
}

void ServerHandler::badRequest()
{
    data_buffer_text = "Сообщение от сервера: Что-то пошло не так. На сервер пришли данные неверной длинны.";
    clearBuffer();
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
        buffer.addFlags(sv::clear_console);
    }
    else
    {
        buffer.removeFlag(sv::clear_console);
    }
}

void ServerHandler::clearBuffer()
{
    // Статические данные
    user->setSessionKey(0);
    buffer.setSessionKey(Misc::getRandomKey());
    buffer.createFlags(sv::get_string);
    buffer.setPaginationMode(sv::last_page);
    buffer.setPgPerPage(10);
    buffer.setPgStart(1);
    buffer.setPgEnd(0);
    // Динамические данные
    buffer.writeDynData("Guest", "MAIN_PAGE", "NONE");
}

std::string &ServerHandler::getDataText()
{
    return data_buffer_text;
}

void ServerHandler::onDBerror()
{
    data_buffer_text = "На сервере проблемы с базой данных. Обратитесь к администратору.\n";
    buffer.createFlags(sv::get_string);
}
