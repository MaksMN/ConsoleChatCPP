#include "ChatGuestPage.h"

ChatGuestPage::ChatGuestPage(DBmessages &_pubMessagesDB,
                             DBmessages &_privMessagesDB,
                             DBcomplaints &_complaintsDB,
                             DBusers &_usersDB,
                             char (&_cmd_buffer)[CMD_BUFFER])
    : IChatInterface(_pubMessagesDB,
                     _privMessagesDB,
                     _complaintsDB,
                     _usersDB,
                     _cmd_buffer)
{
}

void ChatGuestPage::run()
{
    // если пользователь выбрал login
    if (page_text.compare(GUEST_PAGE) == 0 && cmd_text.compare(LOGIN) == 0)
    {
        loginPage();
        return;
    }

    // пользователь ввел данные на странице авторизации
    if (page_text.compare(INPUT_LOGIN_PAGE) == 0)
    {
        validateLogin();
        return;
    }

    // если пользователь выбрал login
    if (page_text.compare(GUEST_PAGE) == 0 && cmd_text.compare(REG) == 0)
    {
        registrationPage();
        return;
    }

    // пользователь ввел данные на странице авторизации
    if (page_text.compare(INPUT_REGISTRATION_PAGE) == 0)
    {
        validateRegistration();
        return;
    }

    // если команды пользователя не подходят ни под один фильтр
    offerRegisterOrLogin();
    return;
}

void ChatGuestPage::offerRegisterOrLogin(const std::string &message)
{
    page_text = GUEST_PAGE;
    std::string str;
    str = message + "Вы не авторизованы.\n"
                    "Доступные команды:\n"
                    "/login - авторизоваться;\n"
                    "/reg - зарегистрироваться;\n"
                    "Введите команду: ";
    data_text = str;
    buffer.addFlags(sv::get_string, sv::clear_console);
    buffer.writeDynData(login, page_text, cmd_text);
}

void ChatGuestPage::loginPage(const std::string &message)
{
    page_text = INPUT_LOGIN_PAGE;
    std::string str;
    str = message + "Введите логин и пароль разделив их двоеточием (логин:пароль).\n"
                    "Введите команду: ";
    data_text = str;
    buffer.createFlags(sv::get_string);
    buffer.writeDynData(login, page_text, cmd_text);
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
    page_text = PUBLIC_PAGE;
    cmd_text = CHAT;
    data_text = "Вы успешно авторизовались в чате.\n";
    buffer.createFlags(sv::get_string, sv::clear_console, sv::no_input);
    buffer.writeDynData(login, page_text, cmd_text);
}

void ChatGuestPage::registrationPage(const std::string &message)
{
    page_text = INPUT_REGISTRATION_PAGE;
    std::string str;
    str = message + "Введите имя, логин и пароль разделив их двоеточием (имя:логин:пароль).\n"
                    "Введите команду: ";
    data_text = str;
    buffer.createFlags(sv::get_string);
    buffer.writeDynData(login, page_text, cmd_text);
}

void ChatGuestPage::validateRegistration()
{
    std::vector<std::string> name_login_pass = Misc::stringExplode(cmd_text, ":");
    if (name_login_pass.size() != 3)
    {
        registrationPage("Вы ввели неверные данные\n");
        return;
    }
    if (name_login_pass[1].size() > 20)
    {
        name_login_pass[1].erase(0, name_login_pass[1].size() - 20);
    }
    if (name_login_pass[0].size() > 20)
    {
        name_login_pass[0].erase(0, name_login_pass[0].size() - 20);
    }
    std::shared_ptr<User> user = usersDB.getUserByLogin(name_login_pass[1]);
    if (user != nullptr || name_login_pass[1] == "Guest")
    {
        registrationPage("Логин " + name_login_pass[1] + " занят\n");
        return;
    }

    user = usersDB.addUser(name_login_pass[1], name_login_pass[0], name_login_pass[2]);
    session_key = Misc::getRandomKey();
    user->setSessionKey(session_key);
    login = user->getLogin();
    std::string str;
    page_text = PUBLIC_PAGE;
    cmd_text = CHAT;
    str = "Вы успешно зарегистрировались и авторизовались в чате.\n";
    data_text = str;
    buffer.createFlags(sv::get_string, sv::clear_console, sv::write_session, sv::no_input);
    buffer.setSessionKey(session_key);
    buffer.writeDynData(login, page_text, cmd_text);
}
