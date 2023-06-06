#include "UserInterface.h"

UserInterface::UserInterface(DBmessages &_pubMessagesDB,
                             DBmessages &_privMessagesDB,
                             DBcomplaints &_complaintsDB,
                             DBusers &_usersDB)
    : IChatInterface(_pubMessagesDB, _privMessagesDB, _complaintsDB, _usersDB) {}

chat::Results UserInterface::run()
{
    UserInput<std::string, chat::Results> chatAreaPage("Страница авторизации и регистрации",
                                                       "Выберите действие (р - Регистрация, вх - Вход, н - Назад, вых - выход): ",
                                                       "Неверный ввод", 4);
    chatAreaPage.addInputs("р", "вх", "н", "вых");
    chatAreaPage.addOutputs(chat::in_register_page, chat::in_login_page, chat::back_, chat::exit_);

    chat::Results userInputResult = chat::user_input;
    do
    {
        system(clear);

        if (userInputResult == chat::in_chat_area)
            userInputResult = chat::user_input;

        if (userInputResult == chat::user_banned)
        {
            std::cout << "Пользователь заблокирован администратором" << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::register_cancel)
        {
            std::cout << "Вы отменили регистрацию" << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::login_cancel)
        {
            std::cout << "Вы отменили вход в чат" << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::user_input)
        {
            AuthorizedUser = nullptr;
            userInputResult = chatAreaPage.IOgetline();
        }

        switch (userInputResult)
        {
        case chat::in_register_page:
            userInputResult = registerPage();
            break;
        case chat::in_login_page:
            userInputResult = loginPage();
            break;
        case chat::in_public_chat_page:
            userInputResult = publicChat();
            break;
        default:
            break;
        }

    } while (userInputResult != chat::exit_ && userInputResult != chat::back_);
    system(clear);
    return userInputResult;
}

chat::Results UserInterface::registerPage()
{
    system(clear);
    std::string login;
    std::string name;
    std::string password;
    std::string incorrectInput = "Неверный ввод. Пустые значения недопустимы.";
    std::cout << "Вы находитесь в меню регистрации" << std::endl;
    UserInput<std::string, std::string> getLogin("Страница регистрации", "Введите логин: ", incorrectInput);
    UserInput<std::string, std::string> getName(std::string(), "Введите отображаемое имя пользователя: ", incorrectInput);
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ", incorrectInput);
    UserInput<std::string, chat::Results> regEnd(std::string(), "Выберите действие (з - Зарегистрироваться, о - Отменить регистрацию): ", "Неверный ввод", 2);
    regEnd.addInputs("з", "о");
    regEnd.addOutputs(chat::register_success, chat::register_cancel);

    // ввод логина
    bool validLogin = false;
    do
    {
        login = getLogin.IOgetlineThrough(true);

        validLogin = usersDB.uniqueLogin(login);
        if (!validLogin)
        {
            std::cout << "Этот логин занят!" << std::endl;
        }
    } while (!validLogin);

    // ввод пароля
    password = getPass.IOgetlineThrough(true);

    // ввод имени
    name = getName.IOgetlineThrough(true);

    // завершение регистрации
    chat::Results endInput;
    endInput = regEnd.IOgetline();
    if (endInput == chat::register_cancel)
    {
        system(clear);
        return chat::register_cancel;
    }

    AuthorizedUser = usersDB.addUser(login, name, password);
    system(clear);
    return chat::in_public_chat_page;
}

chat::Results UserInterface::loginPage()
{
    chat::Results login_result = login();
    system(clear);
    if (login_result == chat::login_success)
        return chat::in_public_chat_page;
    return login_result;
}

chat::Results UserInterface::publicChat()
{
    system(clear);

    std::cout << "Здравствуйте, " << AuthorizedUser->getName() << "!" << std::endl;
    std::cout << "Вы успешно вошли в чат." << std::endl;
    std::cout << std::endl;
    std::string chatDescription;
    std::string mainMessage;

    mainMessage = "Операции в чате: "
                  "\nс - написать сообщение;"
                  "\nн - навигация по чату;"
                  "\nл - личные сообщения;"
                  "\nж - пожаловаться;"
                  "\nп - настройки профиля;"
                  "\nв - выход;"
                  "\nУкажите операцию: ";

    UserInput<std::string, chat::Results> chatMainPage(chatDescription, mainMessage, "Неверный ввод", 6);
    chatMainPage.addInputs("с", "н", "л", "ж", "п", "в");
    chatMainPage.addOutputs(chat::send_message, chat::in_chat_navigation, chat::in_private_chat_page, chat::complaint_, chat::in_user_profile_page, chat::back_);

    chat::Results userInputResult = chat::user_input;
    chat::Pagination msgListMode = chat::Pagination::last_page;
    uint start = 0, perPage = 10, end = 0;

    do
    {
        if (userInputResult == chat::complaint_sended)
        {
            std::cout << "Жалоба успешно отправлена." << std::endl;
            userInputResult = chat::user_input;
        }
        if (userInputResult == chat::in_public_chat_page)
        {
            std::cout << "Вы вернулись в публичный чат." << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::in_chat_area)
        {
            system(clear);
            return userInputResult;
        }

        std::cout << "Вы находитесь в публичном чате.\n"
                  << std::endl;
        if (msgListMode == chat::Pagination::last_page)
        {
            // последние 10 сообщений
            auto m = pubMessagesDB.listLast(start, perPage, end);
            printMessages(m, "В этом чате нет сообщений. Начните общение первым.", ">>>Сообщение №", start);
        }

        if (msgListMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = pubMessagesDB.list(start, perPage, end);
            printMessages(m, "В этом чате нет сообщений. Начните общение первым.", ">>>Сообщение №", start);
        }

        if (!pubMessagesDB.empty())
        {
            std::cout << "Показаны сообщения " << start + 1 << " - " << end << " из " << pubMessagesDB.getCount() << std::endl;
        }
        if (msgListMode == chat::Pagination::element_ && start + perPage < pubMessagesDB.getCount())
        {
            std::cout << "Внимание. Вы не увидите последние сообщения потому-что они находятся за диапазоном отображения." << std::endl;
        }
        std::cout << std::endl;
        if (userInputResult == chat::user_input)
        {
            userInputResult = chatMainPage.IOgetline();
        }
        switch (userInputResult)
        {
        case chat::send_message:
            userInputResult = sendPublicMessage();
            break;
        case chat::in_chat_navigation:
            userInputResult = listNavigation(start, perPage, end, msgListMode, pubMessagesDB.getCount());
            break;
        case chat::complaint_:
            userInputResult = complaint();
            break;
        case chat::in_private_chat_page:
            userInputResult = privateChat();
            break;
        case chat::in_user_profile_page:
            userInputResult = userProfile();
            break;
        default:
            break;
        }
    } while (userInputResult != chat::back_);
    system(clear);
    return chat::user_input;
}
chat::Results UserInterface::privateChat()
{
    system(clear);

    UserInput<std::string, chat::Results> privateMainPage("Личные сообщения. Главная страница.",
                                                          "Выбор пользователя:\n"
                                                          "\tид - указать userid собеседника;\n"
                                                          "\tн - навигация по списку пользователей;\n"
                                                          "\tо - вернуться в общий чат;\n"
                                                          "\tв - выйти из чата\n"
                                                          "Укажите опцию: ",
                                                          "Неверный ввод", 4);
    privateMainPage.addInputs("ид", "н", "о", "в");
    privateMainPage.addOutputs(
        chat::search_user_byId,
        chat::in_chat_navigation,
        chat::in_public_chat_page,
        chat::in_chat_area);

    std::shared_ptr<User> discussant = nullptr;
    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;
    chat::Results userInputResult = chat::user_input;

    do
    {
        if (userInputResult == chat::in_public_chat_page)
        {
            system(clear);
            return userInputResult;
        }
        if (userInputResult == chat::in_private_chat_page)
            userInputResult = chat::user_input;

        if (userInputResult == chat::user_banned)
        {
            std::cout << "Пользователь заблокирован, беседа с ним невозможна." << std::endl;
            userInputResult = chat::user_input;
        }
        std::cout << "Вы находитесь на странице личных сообщений." << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 пользователей
            auto u = usersDB.listLast(start, perPage, end);
            printList(u, "В этом чате нет пользователей. Пригласите других участников в чат.", "Пользователь №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного пользователя
            auto u = usersDB.list(start, perPage, end);
            printList(u, "В этом чате нет пользователей. Пригласите других участников в чат.", "Пользователь №", start);
        }

        if (!pubMessagesDB.empty())
        {
            std::cout << "Показаны пользователи " << start + 1 << " - " << end << " из " << usersDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < pubMessagesDB.getCount())
        {
            std::cout << "Внимание. Вы не увидите последних пользователей потому-что они находятся за диапазоном отображения." << std::endl;
        }
        if (userInputResult == chat::user_input)
            userInputResult = privateMainPage.IOgetline();
        switch (userInputResult)
        {
        case chat::search_user_byId:
            discussant = searchUserByID();
            if (discussant == nullptr)
            {
                userInputResult = chat::user_input;
            }
            else
            {
                userInputResult = privateConversation(discussant);
            }
            break;
        case chat::in_chat_navigation:
            userInputResult = listNavigation(start, perPage, end, listMode, usersDB.getCount());
            break;
        default:
            break;
        }
    } while (userInputResult != chat::in_chat_area && userInputResult != chat::in_public_chat_page);
    system(clear);
    return userInputResult;
}
chat::Results UserInterface::privateConversation(std::shared_ptr<User> discussant)
{
    system(clear);
    if (discussant->isBanned())
        return chat::user_banned;

    std::string mainMessage = "Опции: "
                              "\nс - написать сообщение; "
                              "\nн - навигация по сообщениям; "
                              "\nп - вернуться публичный чат; "
                              "\nв - выход из беседы;"
                              "\nВыберите опцию: ";

    UserInput<std::string, chat::Results> chatConversationPage(std::string(), mainMessage, "Неверный ввод", 4);
    chatConversationPage.addInputs("с", "н", "п", "в");
    chatConversationPage.addOutputs(chat::send_message, chat::in_chat_navigation, chat::in_public_chat_page, chat::in_private_chat_page);

    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;

    chat::Results userInputResult = chat::user_input;

    do
    {
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 сообщений
            auto m = privMessagesDB.getPrivateMsgList(AuthorizedUser->getID(), discussant->getID(), start, perPage, end);
            printMessages(m, "В этом чате нет сообщений. Начните общение первым.", "Сообщение №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = privMessagesDB.getPrivateMsgList(AuthorizedUser->getID(), discussant->getID(), start, perPage, end, false);
            printMessages(m, "В этом чате нет сообщений. Начните общение первым.", "Сообщение №", start);
        }

        if (end > 0)
        {
            std::cout << "Показаны сообщения " << start + 1 << " - " << end << " из " << privMessagesDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < privMessagesDB.getCount())
        {
            std::cout << "Внимание. Вы не увидите последние сообщения, они находятся за диапазоном отображения." << std::endl;
        }
        std::cout << std::endl;

        std::cout << "Личные сообщения. Беседа с пользователем.\n"
                  << std::endl;
        std::cout << "Вы: ";
        AuthorizedUser->printData();
        std::cout << "Ваш собеседник: ";
        discussant->printData();

        if (AuthorizedUser->getID() == discussant->getID())
            std::cout << "Вы ведете беседу с самим собой.\n";

        if (userInputResult == chat::user_input)
            userInputResult = chatConversationPage.IOgetline();

        switch (userInputResult)
        {
        case chat::send_message:
            userInputResult = sendPrivateMessage(discussant);
            break;
        case chat::in_chat_navigation:
            userInputResult = listNavigation(start, perPage, end, listMode, end);
            break;

        default:
            break;
        }
    } while (userInputResult != chat::in_private_chat_page && userInputResult != chat::in_public_chat_page);
    system(clear);
    return userInputResult;
}
chat::Results UserInterface::sendPublicMessage()
{
    std::string messageText;
    UserInput<std::string, std::string> send(std::string(), "Введите текст сообщения: ", "Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);
    pubMessagesDB.addMessage(AuthorizedUser->getID(), AuthorizedUser->getID(), messageText, msg::public_);
    system(clear);
    return chat::user_input;
}

chat::Results UserInterface::sendPrivateMessage(std::shared_ptr<User> discussant)
{
    std::string messageText;
    UserInput<std::string, std::string> send(std::string(), "Введите текст сообщения: ", "Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);
    privMessagesDB.addMessage(AuthorizedUser->getID(), discussant->getID(), messageText, msg::public_);
    system(clear);
    return chat::user_input;
}

chat::Results UserInterface::complaint()
{
    UserInput<std::string, std::string> complaintTextIO(std::string(), "Укажите причину жалобы: ", std::string());
    UserInput<int, int> troubleMsgIdIO(std::string(), "Укажите msgid сообщения на которое хотите пожаловаться: ", std::string());
    UserInput<std::string, chat::Results> yesnoIO(std::string(), "Отменить жалобу? (да - отменить / нет - не отменять): ", "Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs("да", "нет", "yes", "no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    std::string text = complaintTextIO.IOgetlineThrough(true);
    std::shared_ptr<Message> troubleMsg = nullptr;

    do
    {
        uint troubleMsgId = troubleMsgIdIO.IOcinThrough();
        troubleMsg = pubMessagesDB.getMessageByID(troubleMsgId);
        if (troubleMsg == nullptr)
        {
            std::cout << "Указан неверный msgid" << std::endl;
            chat::Results yesno = yesnoIO.IOgetline();
            if (yesno == chat::yes_)
            {
                return chat::user_input;
            }
            else if (yesno == chat::no_)
            {
                continue;
            }
        }
    } while (troubleMsg == nullptr);

    auto defendantUser = usersDB.getUserByID(troubleMsg->getAuthorID());
    std::string complaintText = "\nЖалоба от пользователя: " +
                                AuthorizedUser->getName() +
                                "[" + AuthorizedUser->getLogin() + "][userid " + std::to_string(AuthorizedUser->getID()) + "]\n" +
                                "На пользователя: " +
                                defendantUser->getName() +
                                "[" + defendantUser->getLogin() + "][userid " + std::to_string(defendantUser->getID()) + "]\n" +
                                "Текст жалобы: " + text +
                                "\nЗа сообщение: [msgid " + std::to_string(troubleMsg->getID()) + "]. Текст сообщения: " + troubleMsg->getText() + "\n";

    yesnoIO.setDescription(complaintText);
    yesnoIO.setMainMessage("Вы действительно хотите отправить эту жалобу администрации чата? (да / нет): ");

    chat::Results yesno = yesnoIO.IOgetline();
    if (yesno == chat::no_)
    {
        system(clear);
        return chat::user_input;
    }
    complaintsDB.addComplaint(troubleMsg->getID(), AuthorizedUser->getID(), defendantUser->getID(), complaint::status::open_, complaintText, troubleMsg->getText());
    system(clear);
    return chat::complaint_sended;
}
std::shared_ptr<User> UserInterface::searchUserByID()
{
    std::shared_ptr<User> user = nullptr;

    UserInput<uint, uint> searchByIdInput("Поиск пользователя по userid", "Введите userid: ", "Неверный ввод");

    UserInput<std::string, chat::Results> yesnoIO("Указан неверный userid", "Отменить писк пользователя? (да - отменить / нет - не отменять): ", "Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs("да", "нет", "yes", "no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    do
    {
        uint id = searchByIdInput.IOcinThrough();
        user = usersDB.getUserByID(id);
        if (user == nullptr)
        {
            chat::Results yesno = yesnoIO.IOgetline();
            if (yesno == chat::yes_)
            {
                return nullptr;
            }
            else if (yesno == chat::no_)
            {
                continue;
            }
        }

    } while (user == nullptr);

    return user;
}
chat::Results UserInterface::userProfile()
{
    system(clear);
    UserInput<std::string, user::options> options("Настройки пользователя. " + AuthorizedUser->getName() + "[" + AuthorizedUser->getLogin() + "]",
                                                  "Какие данные вы хотите поменять? (п - пароль; и - имя; з - закончить): ",
                                                  "Неверный ввод",
                                                  3);
    options.addInputs("п", "и", "з");
    options.addOutputs(user::set_pass, user::set_name, user::complete_);

    UserInput<std::string, std::string> getLogin("Изменение логина", "Введите логин: ", std::string());
    UserInput<std::string, std::string> getName("Изменение имени", "Введите имя: ", std::string());
    UserInput<std::string, std::string> getPass("Изменение пароля", "Введите пароль: ", std::string());
    UserInput<std::string, chat::Results> loginCancel(std::string(), "Отменить смену логина? (да - отменить, нет - продолжить): ", "Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs("да", "нет", "yes", "no");
    loginCancel.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    std::string loginTemp;
    std::string name;
    std::string pass;
    chat::Results userInputResult = chat::user_input;
    do
    {
        options.setDescription("Настройки пользователя. " + AuthorizedUser->getName() + "[" + AuthorizedUser->getLogin() + "]");
        user::options result = options.IOgetline();
        switch (result)
        {
        case user::set_name:
            name = getName.IOgetlineThrough(true);
            AuthorizedUser->setName(name);
            usersDB.updateFiles();
            break;

        case user::set_pass:
            pass = getPass.IOgetlineThrough(true);
            AuthorizedUser->setPass(pass);
            usersDB.updateFiles();
            break;
        case user::complete_:
            system(clear);
            return chat::in_public_chat_page;
        default:
            break;
        }
        system(clear);
    } while (1);
};
