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
    Misc::writeStringBuffer("S", cmd_buffer, 0, false);
    cmd_buffer[1] = 1;
    writeBuffer();
}

void ChatGuestPage::loginPage()
{
    page_text = "INPUT_LOGIN_PAGE";
    std::string str;
    str = "Введите логин и пароль разделив их двоеточием (логин:пароль).\n"
          "Введите команду: ";
    Misc::writeStringBuffer(str, data_buffer);
    Misc::writeStringBuffer("S", cmd_buffer, 0, false);
    cmd_buffer[1] = 0;
    writeBuffer();
}

void ChatGuestPage::validateLogin()
{
    std::vector<std::string> login_pass = Misc::stringExplode(cmd_text, ":");
    if (login_pass.size() != 2)
    {
        offerRegisterOrLogin("Вы ввели неверные данные\n");
        return;
    }

    std::shared_ptr<User> user = usersDB.getUserByLogin(login_pass[0]);
    if (user == nullptr)
    {
        offerRegisterOrLogin("Пользователь с логином " + login_pass[0] + " не существует  \n");
        return;
    }

    if (!user->validatePass(login_pass[1]))
    {
        offerRegisterOrLogin("Вы ввели неверный пароль\n");
        return;
    }

    user->setSessionKey(session_key);
    login = user->getLogin();
    page_text = "PUBLIC_CHAT_PAGE";
    std::string str;
    str = "Вы успешно авторизовались в чате.\n"
          "Введите команду chat: ";
    Misc::writeStringBuffer(str, data_buffer);

    Misc::writeStringBuffer("S", cmd_buffer, 0, false);
    cmd_buffer[1] = 1;
    writeBuffer();
}
