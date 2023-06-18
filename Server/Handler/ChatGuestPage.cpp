#include "ChatGuestPage.h"

ChatGuestPage::ChatGuestPage(DBmessages &_pubMessagesDB,
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
                     _cmd_buffer) {}

void ChatGuestPage::run()
{
    // если пользователь выбрал login
    if (page_text.compare("GUEST_PAGE") == 0 && cmd_text.compare("login") == 0)
    {
        loginPage();
        return;
    }

    // пользователь ввел данные на странице авторизации
    if (page_text.compare("INPUT_LOGIN_PAGE") == 0)
    {
        validateLogin();
        return;
    }

    // если пользователь выбрал login
    if (page_text.compare("GUEST_PAGE") == 0 && cmd_text.compare("reg") == 0)
    {
        registrationPage();
        return;
    }

    // пользователь ввел данные на странице авторизации
    if (page_text.compare("INPUT_REGISTRATION_PAGE") == 0)
    {
        validateRegistration();
        return;
    }

    // если команды пользователя не подходят ни под один фильтр
    offerRegisterOrLogin();
    return;
}

void ChatGuestPage::offerRegisterOrLogin(std::string message)
{
    page_text = "GUEST_PAGE";
    std::string str;
    str = message + "Вы не авторизованы.\n"
                    "Доступные команды:\n"
                    "login - авторизоваться;\n"
                    "reg - зарегистрироваться;\n"
                    "Введите команду: ";
    Misc::writeStringBuffer(str, data_buffer);
    cmd_buffer[0] = netOptions.create(sv::get_string, sv::clear_console);
    writeBuffer();
}

void ChatGuestPage::loginPage(std::string message)
{
    page_text = "INPUT_LOGIN_PAGE";
    std::string str;
    str = message + "Введите логин и пароль разделив их двоеточием (логин:пароль).\n"
                    "Введите команду: ";
    Misc::writeStringBuffer(str, data_buffer);
    cmd_buffer[0] = netOptions.create(sv::get_string);
    writeBuffer();
}

void ChatGuestPage::validateLogin()
{
    std::vector<std::string> login_pass = Misc::stringExplode(cmd_text, ":");
    if (login_pass.size() != 2)
    {
        loginPage("Вы ввели неверные данные\n");
        return;
    }

    std::shared_ptr<User> user = usersDB.getUserByLogin(login_pass[0]);
    if (user == nullptr)
    {
        loginPage("Пользователь с логином " + login_pass[0] + " не существует  \n");
        return;
    }

    if (!user->validatePass(login_pass[1]))
    {
        loginPage("Вы ввели неверный пароль\n");
        return;
    }

    user->setSessionKey(session_key);
    login = user->getLogin();
    page_text = "MAIN_PAGE";
    std::string str;
    str = "Вы успешно авторизовались в чате.\n"
          "Введите команду /chat: ";
    Misc::writeStringBuffer(str, data_buffer);
    cmd_buffer[0] = netOptions.create(sv::get_string, sv::clear_console);
    writeBuffer();
}

void ChatGuestPage::registrationPage(std::string message)
{
    page_text = "INPUT_REGISTRATION_PAGE";
    std::string str;
    str = message + "Введите имя, логин и пароль разделив их двоеточием (имя:логин:пароль).\n"
                    "Введите команду: ";
    Misc::writeStringBuffer(str, data_buffer);
    cmd_buffer[0] = netOptions.create(sv::get_string);
    writeBuffer();
}

void ChatGuestPage::validateRegistration()
{
    std::vector<std::string> name_login_pass = Misc::stringExplode(cmd_text, ":");
    if (name_login_pass.size() != 3)
    {
        registrationPage("Вы ввели неверные данные\n");
        return;
    }

    std::shared_ptr<User> user = usersDB.getUserByLogin(name_login_pass[1]);
    if (user != nullptr)
    {
        registrationPage("Логин " + name_login_pass[1] + " занят\n");
        return;
    }

    if (user == nullptr)
    {
        user = usersDB.addUser(name_login_pass[1], name_login_pass[0], name_login_pass[2]);
        session_key = Misc::getRandomKey();
        user->setSessionKey(session_key);
        login = user->getLogin();
        std::string str;
        page_text = "MAIN_PAGE";
        str = "Вы успешно зарегистрировались и авторизовались в чате.\n"
              "Введите команду /chat: ";
        Misc::writeStringBuffer(str, data_buffer);
        cmd_buffer[0] = netOptions.create(sv::get_string, sv::clear_console, sv::write_session);
        Misc::writeUlongBuffer(session_key, cmd_buffer, 2);
        writeBuffer();
    }
}
