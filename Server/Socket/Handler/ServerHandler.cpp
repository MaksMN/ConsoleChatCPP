#include "ServerHandler.h"

ServerHandler::ServerHandler(char (&_cmd_buffer)[CMD_BUFFER]) : cmd_buffer(_cmd_buffer) {}

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

    // данные пользователя
    user = usersDB.getUserByLogin(login);
    if ((user != nullptr && user->getSessionKey() != session_key && session_key != 0) || user == nullptr)
    {
        user = nullptr;
        login = "Guest";
    }

    // Запишем в буфер данные на случай если при обработке данные не изменятся.
    data_buffer_text = "Вы ввели неизвестную команду.\nВведите команду /chat: ";
    // изменим флаги чтобы при неизвестной команде клиент встал с запросом
    buffer.createFlags(sv::get_string);

    /* Общие команды чата, которые срабатывают в любом месте для всех */
    if (cmd_text == CHAT)
    {
        buffer.writeDynData(login, MAIN_PAGE, CHAT);
        page_text = MAIN_PAGE;
    }

    if (cmd_text == HELLO)
    {
        data_buffer_text = "Привет, " + login + "! Я сервер, я живой.\nВведите команду: ";
        clearConsole(false);
        return;
    }
    if (cmd_text == HELP)
    {
        data_buffer_text =
            "\nКоманды, которые можно вызвать в любое время:\n"
            "Команда /chat - перейти на главную страницу из любого раздела;\n"
            "Команда /hello - опрос сервера;\n"
            "Команда /help - справка;\n"
            "Команда /quit - закрыть клиент;\n"
            "\n"
            "Команды, которые можно вызвать в любое время авторизованным пользователям:\n"
            "Команда /profile - сменить имя или пароль;\n"
            "Команда /logout - выйти из чата;\n"
            "Команда /sv_quit - (admin)завершить работу сервера;\n"
            "\nВведите команду: ";
        clearConsole(false);
        return;
    }

    /* проверка соответствия команд карте чата */
    if (!chatMap.checkPage(page_text, cmd_text))
        return;

    /* Авторизация и регистрация */
    /* Сюда попадает любой неавторизованный пользователь */
    if (user == nullptr)
    {
        ChatGuestPage guestPage{pubMessagesDB,
                                privMessagesDB,
                                complaintsDB,
                                usersDB,
                                cmd_buffer};
        guestPage.run();
        data_buffer_text = guestPage.getDataText();
        return;
    }

    /* Команды чата которые срабатывают если пользователь авторизован */

    // закрыть сервер (только админ)
    if (cmd_text.compare(SV_QUIT) == 0)
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
    if (cmd_text == LOGOUT)
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
    pages_set.clear();
    pages_set.insert(PROFILE_PAGE);
    pages_set.insert(PROFILE_PAGE_INPUT);
    if (cmd_text == PROFILE || pages_set.contains(page_text))
    {
        ChatEditProfile editProfile{pubMessagesDB,
                                    privMessagesDB,
                                    complaintsDB,
                                    usersDB,
                                    cmd_buffer};
        editProfile.run();
        data_buffer_text = editProfile.getDataText();
        return;
    }

    // главная страница чата
    pages_set.clear();
    pages_set.insert(PUBLIC_PAGE);
    pages_set.insert(MAIN_PAGE);
    pages_set.insert(PUBLIC_PAGE_INPUT);
    if (pages_set.contains(page_text))
    {
        ChatPublicPage publicPage{pubMessagesDB,
                                  privMessagesDB,
                                  complaintsDB,
                                  usersDB,
                                  cmd_buffer};
        publicPage.run();
        data_buffer_text = publicPage.getDataText();
        return;
    }

    // Список пользователей в приватном чате
    pages_set.clear();
    pages_set.insert(PRIVATE_PAGE_USERS);
    pages_set.insert(PRIVATE_PAGE_USERS_INPUT);
    if (pages_set.contains(page_text))
    {
        ChatPrivatePageUsers privateUsersPage{pubMessagesDB,
                                              privMessagesDB,
                                              complaintsDB,
                                              usersDB,
                                              cmd_buffer};
        privateUsersPage.run();
        data_buffer_text = privateUsersPage.getDataText();
        return;
    }

    // Сообщения в приватном чате
    pages_set.clear();
    pages_set.insert(PRIVATE_PAGE_MESSAGES);
    pages_set.insert(PRIVATE_PAGE_MESSAGES_INPUT);
    if (pages_set.contains(page_text))
    {
        ChatPrivatePageMessages privateMessagesPage{pubMessagesDB,
                                                    privMessagesDB,
                                                    complaintsDB,
                                                    usersDB,
                                                    cmd_buffer};
        privateMessagesPage.run();
        data_buffer_text = privateMessagesPage.getDataText();
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
