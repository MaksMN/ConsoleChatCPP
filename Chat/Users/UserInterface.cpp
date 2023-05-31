#include "UserInterface.h"

UserInterface::UserInterface(DBmessages &_pubMessagesDB,
                             DBmessages &_privMessagesDB,
                             DBcomplaints &_complaintsDB,
                             DBusers &_usersDB)
    : IChatInterface(_pubMessagesDB, _privMessagesDB, _complaintsDB, _usersDB) {}

chat::Results UserInterface::run()
{
    UserInput<std::wstring, chat::Results> chatAreaPage(L"Страница авторизации и регистрации",
                                                        L"Выберите действие (р - Регистрация, вх - Вход, н - Назад, вых - выход): ",
                                                        L"Неверный ввод", 4);
    chatAreaPage.addInputs(L"р", L"вх", L"н", L"вых");
    chatAreaPage.addOutputs(chat::in_register_page, chat::in_login_page, chat::back_, chat::exit_);

    chat::Results userInputResult = chat::user_input;
    do
    {
        system(clear);

        if (userInputResult == chat::in_chat_area)
            userInputResult = chat::user_input;

        if (userInputResult == chat::user_banned)
        {
            std::wcout << L"Пользователь заблокирован администратором" << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::register_cancel)
        {
            std::wcout << L"Вы отменили регистрацию" << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::login_cancel)
        {
            std::wcout << L"Вы отменили вход в чат" << std::endl;
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
    std::wstring login;
    std::wstring name;
    std::wstring password;
    std::wstring incorrectInput = L"Неверный ввод. Пустые значения недопустимы.";
    std::wcout << L"Вы находитесь в меню регистрации" << std::endl;
    UserInput<std::wstring, std::wstring> getLogin(L"Страница регистрации", L"Введите логин: ", incorrectInput);
    UserInput<std::wstring, std::wstring> getName(std::wstring(), L"Введите отображаемое имя пользователя: ", incorrectInput);
    UserInput<std::wstring, std::wstring> getPass(std::wstring(), L"Введите пароль: ", incorrectInput);
    UserInput<std::wstring, chat::Results> regEnd(std::wstring(), L"Выберите действие (з - Зарегистрироваться, о - Отменить регистрацию): ", L"Неверный ввод", 2);
    regEnd.addInputs(L"з", L"о");
    regEnd.addOutputs(chat::register_success, chat::register_cancel);

    // ввод логина
    bool validLogin = false;
    do
    {
        login = getLogin.IOgetlineThrough(true);

        validLogin = usersDB.uniqueLogin(login);
        if (!validLogin)
        {
            std::wcout << L"Этот логин занят!" << std::endl;
        }
    } while (!validLogin);

    // ввод пароля
    password = getPass.IOgetlineThrough(true, false);

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

    std::wcout << L"Здравствуйте, " << AuthorizedUser->getName() << L"!" << std::endl;
    std::wcout << L"Вы успешно вошли в чат." << std::endl;
    std::wcout << std::endl;
    std::wstring chatDescription;
    std::wstring mainMessage;

    mainMessage = L"Операции в чате: "
                  L"\nс - написать сообщение;"
                  L"\nн - навигация по чату;"
                  L"\nл - личные сообщения;"
                  L"\nж - пожаловаться;"
                  L"\nп - настройки профиля;"
                  L"\nв - выход;"
                  L"\nУкажите операцию: ";

    UserInput<std::wstring, chat::Results> chatMainPage(chatDescription, mainMessage, L"Неверный ввод", 6);
    chatMainPage.addInputs(L"с", L"н", L"л", L"ж", L"п", L"в");
    chatMainPage.addOutputs(chat::send_message, chat::in_chat_navigation, chat::in_private_chat_page, chat::complaint_, chat::in_user_profile_page, chat::back_);

    chat::Results userInputResult = chat::user_input;
    chat::Pagination msgListMode = chat::Pagination::last_page;
    uint start = 0, perPage = 10, end = 0;

    do
    {
        if (userInputResult == chat::complaint_sended)
        {
            std::wcout << L"Жалоба успешно отправлена." << std::endl;
            userInputResult = chat::user_input;
        }
        if (userInputResult == chat::in_public_chat_page)
        {
            std::wcout << L"Вы вернулись в публичный чат." << std::endl;
            userInputResult = chat::user_input;
        }

        if (userInputResult == chat::in_chat_area)
        {
            system(clear);
            return userInputResult;
        }

        std::wcout << L"Вы находитесь в публичном чате.\n"
                   << std::endl;
        if (msgListMode == chat::Pagination::last_page)
        {
            // последние 10 сообщений
            auto m = pubMessagesDB.listLast(start, perPage, end);
            printMessages(m, L"В этом чате нет сообщений. Начните общение первым.", L">>>Сообщение №", start);
        }

        if (msgListMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = pubMessagesDB.list(start, perPage, end);
            printMessages(m, L"В этом чате нет сообщений. Начните общение первым.", L">>>Сообщение №", start);
        }

        if (!pubMessagesDB.empty())
        {
            std::wcout << L"Показаны сообщения " << start + 1 << L" - " << end << L" из " << pubMessagesDB.getCount() << std::endl;
        }
        if (msgListMode == chat::Pagination::element_ && start + perPage < pubMessagesDB.getCount())
        {
            std::wcout << L"Внимание. Вы не увидите последние сообщения потому-что они находятся за диапазоном отображения." << std::endl;
        }
        std::wcout << std::endl;
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

    UserInput<std::wstring, chat::Results> privateMainPage(L"Личные сообщения. Главная страница.",
                                                           L"Выбор пользователя:\n"
                                                           L"\tид - указать userid собеседника;\n"
                                                           L"\tн - навигация по списку пользователей;\n"
                                                           L"\tо - вернуться в общий чат;\n"
                                                           L"\tв - выйти из чата\n"
                                                           L"Укажите опцию: ",
                                                           L"Неверный ввод", 4);
    privateMainPage.addInputs(L"ид", L"н", L"о", L"в");
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
            std::wcout << L"Пользователь заблокирован, беседа с ним невозможна." << std::endl;
            userInputResult = chat::user_input;
        }
        std::wcout << L"Вы находитесь на странице личных сообщений." << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 пользователей
            auto u = usersDB.listLast(start, perPage, end);
            printList(u, L"В этом чате нет пользователей. Пригласите других участников в чат.", L"Пользователь №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного пользователя
            auto u = usersDB.list(start, perPage, end);
            printList(u, L"В этом чате нет пользователей. Пригласите других участников в чат.", L"Пользователь №", start);
        }

        if (!pubMessagesDB.empty())
        {
            std::wcout << L"Показаны пользователи " << start + 1 << L" - " << end << L" из " << usersDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < pubMessagesDB.getCount())
        {
            std::wcout << L"Внимание. Вы не увидите последних пользователей потому-что они находятся за диапазоном отображения." << std::endl;
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

    std::wstring mainMessage = L"Опции: "
                               L"\nс - написать сообщение; "
                               L"\nн - навигация по сообщениям; "
                               L"\nп - вернуться публичный чат; "
                               L"\nв - выход из беседы;"
                               L"\nВыберите опцию: ";

    UserInput<std::wstring, chat::Results> chatConversationPage(std::wstring(), mainMessage, L"Неверный ввод", 4);
    chatConversationPage.addInputs(L"с", L"н", L"п", L"в");
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
            printMessages(m, L"В этом чате нет сообщений. Начните общение первым.", L"Сообщение №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = privMessagesDB.getPrivateMsgList(AuthorizedUser->getID(), discussant->getID(), start, perPage, end, false);
            printMessages(m, L"В этом чате нет сообщений. Начните общение первым.", L"Сообщение №", start);
        }

        if (end > 0)
        {
            std::wcout << L"Показаны сообщения " << start + 1 << L" - " << end << L" из " << privMessagesDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < privMessagesDB.getCount())
        {
            std::wcout << L"Внимание. Вы не увидите последние сообщения, они находятся за диапазоном отображения." << std::endl;
        }
        std::wcout << std::endl;

        std::wcout << L"Личные сообщения. Беседа с пользователем.\n"
                   << std::endl;
        std::wcout << L"Вы: ";
        AuthorizedUser->printData();
        std::wcout << L"Ваш собеседник: ";
        discussant->printData();

        if (AuthorizedUser->getID() == discussant->getID())
            std::wcout << L"Вы ведете беседу с самим собой.\n";

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
    std::wstring messageText;
    UserInput<std::wstring, std::wstring> send(std::wstring(), L"Введите текст сообщения: ", L"Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);
    pubMessagesDB.addMessage(AuthorizedUser->getID(), AuthorizedUser->getID(), messageText, msg::public_);
    system(clear);
    return chat::user_input;
}

chat::Results UserInterface::sendPrivateMessage(std::shared_ptr<User> discussant)
{
    std::wstring messageText;
    UserInput<std::wstring, std::wstring> send(std::wstring(), L"Введите текст сообщения: ", L"Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);
    privMessagesDB.addMessage(AuthorizedUser->getID(), discussant->getID(), messageText, msg::public_);
    system(clear);
    return chat::user_input;
}

chat::Results UserInterface::complaint()
{
    UserInput<std::wstring, std::wstring> complaintTextIO(std::wstring(), L"Укажите причину жалобы: ", std::wstring());
    UserInput<int, int> troubleMsgIdIO(std::wstring(), L"Укажите msgid сообщения на которое хотите пожаловаться: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), L"Отменить жалобу? (да - отменить / нет - не отменять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    std::wstring text = complaintTextIO.IOgetlineThrough(true);
    std::shared_ptr<Message> troubleMsg = nullptr;

    do
    {
        uint troubleMsgId = troubleMsgIdIO.IOcinThrough();
        troubleMsg = pubMessagesDB.getMessageByID(troubleMsgId);
        if (troubleMsg == nullptr)
        {
            std::wcout << L"Указан неверный msgid" << std::endl;
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
    std::wstring complaintText = L"\nЖалоба от пользователя: " +
                                 AuthorizedUser->getName() +
                                 L"[" + AuthorizedUser->getLogin() + L"][userid " + std::to_wstring(AuthorizedUser->getID()) + L"]\n" +
                                 L"На пользователя: " +
                                 defendantUser->getName() +
                                 L"[" + defendantUser->getLogin() + L"][userid " + std::to_wstring(defendantUser->getID()) + L"]\n" +
                                 L"Текст жалобы: " + text +
                                 L"\nЗа сообщение: [msgid " + std::to_wstring(troubleMsg->getID()) + L"]. Текст сообщения: " + troubleMsg->getText() + L"\n";

    yesnoIO.setDescription(complaintText);
    yesnoIO.setMainMessage(L"Вы действительно хотите отправить эту жалобу администрации чата? (да / нет): ");

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

    UserInput<uint, uint> searchByIdInput(L"Поиск пользователя по userid", L"Введите userid: ", L"Неверный ввод");

    UserInput<std::wstring, chat::Results> yesnoIO(L"Указан неверный userid", L"Отменить писк пользователя? (да - отменить / нет - не отменять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
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
    UserInput<std::wstring, user::options> options(L"Настройки пользователя. " + AuthorizedUser->getName() + L"[" + AuthorizedUser->getLogin() + L"]",
                                                   L"Какие данные вы хотите поменять? (п - пароль; и - имя; з - закончить): ",
                                                   L"Неверный ввод",
                                                   3);
    options.addInputs(L"п", L"и", L"з");
    options.addOutputs(user::set_pass, user::set_name, user::complete_);

    UserInput<std::wstring, std::wstring> getLogin(L"Изменение логина", L"Введите логин: ", std::wstring());
    UserInput<std::wstring, std::wstring> getName(L"Изменение имени", L"Введите имя: ", std::wstring());
    UserInput<std::wstring, std::wstring> getPass(L"Изменение пароля", L"Введите пароль: ", std::wstring());
    UserInput<std::wstring, chat::Results> loginCancel(std::wstring(), L"Отменить смену логина? (да - отменить, нет - продолжить): ", L"Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs(L"да", L"нет", L"yes", L"no");
    loginCancel.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    std::wstring loginTemp;
    std::wstring name;
    std::wstring pass;
    chat::Results userInputResult = chat::user_input;
    do
    {
        options.setDescription(L"Настройки пользователя. " + AuthorizedUser->getName() + L"[" + AuthorizedUser->getLogin() + L"]");
        user::options result = options.IOgetline();
        switch (result)
        {
        case user::set_name:
            name = getName.IOgetlineThrough(true);
            AuthorizedUser->setName(name);
            break;

        case user::set_pass:
            pass = getPass.IOgetlineThrough(true);
            AuthorizedUser->setPass(pass);
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
