#include "ChatUserInterface.h"

chat::Results ChatUserInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    chat::Results userInput = chat::empty;
    UserInput<std::wstring, chat::Results> chatAreaPage(L"Страница авторизации и регистрации",
                                                       L"Выберите действие (р - Регистрация, вх - Вход, н - Назад, вых - выход): ",
                                                       L"Неверный ввод", 4);
    chatAreaPage.addInputs(L"р", L"вх", L"н", L"вых");
    chatAreaPage.addOutputs(chat::registration, chat::login, chat::back, chat::app_exit);
    do
    {
        system(clear);
        if (userInput == chat::user_banned)
        {
            std::wcout << L"Пользователь заблокирован администратором" << std::endl;
        }
        userInput = chatAreaPage.IOgetline();
        switch (userInput)
        {
        case chat::registration:
            userInput = registration();
            break;
        case chat::login:
            userInput = loginInChat();
            break;
        case chat::back:
            userInput = chat::back;
            user = nullptr;
            break;
        case chat::app_exit:
            userInput = chat::app_exit;
            user = nullptr;
            break;
        default:
            break;
        }
        pgDefault();
    } while (userInput != chat::app_exit && userInput != chat::back);
    return userInput;
}

chat::Results ChatUserInterface::loginInChat()
{
    auto result = login();
    if (result == chat::login_success)
    {
        return publicChat();
    }

    return result;
}

chat::Results ChatUserInterface::registration()
{
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

        validLogin = db->isUniqueLogin(login);
        if (!validLogin)
        {
            std::wcout << L"Этот логин занят!" << std::endl;
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
        return chat::register_cancel;
    }

    // При удачном завершении регистрации - переход в чат
    User _user(name, login, password);
    db->addUser(_user);
    user = std::make_unique<User>(_user);
    return publicChat();
}

chat::Results ChatUserInterface::publicChat()
{
    system(clear);
    std::wcout << std::endl;
    std::wcout << L"Здравствуйте, " << user->getUserName() << L"!" << std::endl;
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
    chatMainPage.addOutputs(chat::send_message, chat::chat_options, chat::private_chat, chat::complaint, chat::user_profile, chat::back);

    chat::Results result = chat::empty;

    do
    {
        auto messages = db->getAllPublicMessages(pg_MaxItems);

        if (messages == nullptr)
        {
            std::wcout << L"В этом чате нет сообщений. Начните общение первым." << std::endl;
        }
        messagesList(std::move(messages));

        std::wcout << std::endl;
        chatDescription = user->getUserName() + L" [" + user->getUserLogin() +
                          L"] Общий чат. Показаны сообщения: " +
                          std::to_wstring((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + L" - " +
                          std::to_wstring(pg_EndItem) + L" из " +
                          std::to_wstring(pg_MaxItems);
        chatMainPage.setDescription(chatDescription);

        result = chatMainPage.IOgetline();
        switch (result)
        {
        case chat::send_message:
            sendMessage();
            break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::private_chat:
            result = privateChat();
            break;
        case chat::complaint:
            complaint();
            break;
        case chat::user_profile:
            userProfile();
            break;
        default:
            break;
        }
        system(clear);
    } while (result != chat::back);
    return chat::empty;
}

void ChatUserInterface::sendMessage(chat::Results result)
{
    Message message;
    std::wstring messageText;
    UserInput<std::wstring, std::wstring> send(std::wstring(), L"Введите текст сообщения: ", L"Неверный ввод. Пустые значения недопустимы.");
    messageText = send.IOgetlineThrough(true);

    message.setAuthorID(user->getId());
    message.setMessage(messageText);
    if (result == chat::public_chat)
    {
        db->AddMessageToAllUsers(message);
        return;
    }
    if (result == chat::private_chat)
    {
        message.setRecipientID(pm_user->getId());
        db->addMessage(message);
    }
}

chat::Results ChatUserInterface::privateChat()
{
    system(clear);
    pgDefault();
    UserInput<std::wstring, chat::Results> privateMainPage(L"Личные сообщения. Главная страница.",
                                                          L"Выбор пользователя:\n"
                                                          L"\tл - по логину;\n"
                                                          L"\tи - по userID;\n"
                                                          L"Другие опции:\n"
                                                          L"\tн - навигация по списку пользователей;\n"
                                                          L"\tо - вернуться в общий чат;\n"
                                                          L"\tв - выйти из чата\n"
                                                          L"Укажите опцию: ",
                                                          L"Неверный ввод", 5);
    privateMainPage.addInputs(L"л", L"и", L"н", L"о", L"в");
    privateMainPage.addOutputs(chat::search_user_byLogin,
                               chat::search_user_byId,
                               chat::user_list,
                               chat::public_chat,
                               chat::back);
    chat::Results result = chat::private_chat;
    do
    {
        std::wstring chatDescription;

        if (result == chat::public_chat)
        {
            return chat::public_chat;
        }
        system(clear);
        auto users = db->getAllUsers();
        if (users == nullptr)
        {
            std::wcout << L"В этом чате нет пользователей. Пригласите других участников." << std::endl;
        }

        usersList(std::move(users));
        switch (result)
        {
        case chat::user_not_found:
            std::wcout << L"Пользователь не найден." << std::endl;
            break;
        case chat::user_banned:
            std::wcout << L"Пользователь заблокирован администратором." << std::endl;
            break;
        case chat::user_is_service:
            std::wcout << L"C этим пользователем нельзя вести личные беседы." << std::endl;
            break;

        default:
            break;
        }

        chatDescription = L"Личные сообщения. Главная страница.\n"
                          L"Вы: " +
                          user->getUserName() + L" [" + user->getUserLogin() + L"]\t[userID " + std::to_wstring(user->getId()) + L"]\n" +
                          L"Показаны пользователи: " +
                          std::to_wstring((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + L" - " +
                          std::to_wstring(pg_EndItem) + L" из " +
                          std::to_wstring(pg_MaxItems);

        privateMainPage.setDescription(chatDescription);
        result = privateMainPage.IOgetline();

        switch (result)
        {
        case chat::search_user_byLogin:
            result = privateChatWithUser(chat::search_user_byLogin);
            break;
        case chat::search_user_byId:
            result = privateChatWithUser(chat::search_user_byId);
            break;
        case chat::user_list:
            pgNavigation();
            break;
        case chat::public_chat:
            result = chat::public_chat;
            break;
        case chat::back:
            result = chat::back;
            break;
        default:
            break;
        }
    } while ((result != chat::public_chat) && (result != chat::back));

    pgDefault();
    pm_user = nullptr;
    return result;
}

void ChatUserInterface::searchUser(chat::Results results)
{
    UserInput<int, int> searchByIdInput(L"Писк пользователя по userID", L"Введите userID: ", L"Неверный ввод");
    UserInput<std::wstring, std::wstring> searchByLoginInput(L"Писк пользователя по логину", L"Введите логин пользователя: ", L"Неверный ввод");
    if (results == chat::search_user_byId)
    {
        int uid = searchByIdInput.IOcinThrough();
        std::unique_ptr<User> _user = db->getUserById(uid);
        pm_user = std::move(_user);
    }
    if (results == chat::search_user_byLogin)
    {
        std::wstring login = searchByLoginInput.IOgetlineThrough();
        std::unique_ptr<User> _user = db->getUserByLogin(login);
        pm_user = std::move(_user);
    }
}

chat::Results ChatUserInterface::privateChatWithUser(chat::Results result)
{
    searchUser(result);
    if (pm_user == nullptr)
    {
        return chat::user_not_found;
    }

    system(clear);
    pgDefault();
    std::wstring chatDescription;
    std::wstring mainMessage = L"Опции: "
                              L"\nс - написать сообщение; "
                              L"\nн - навигация по сообщениям; "
                              L"\nп - вернуться публичный чат; "
                              L"\nв - выход из беседы;"
                              L"\nВыберите опцию: ";

    UserInput<std::wstring, chat::Results> chatConversationPage(chatDescription, mainMessage, L"Неверный ввод", 4);
    chatConversationPage.addInputs(L"с", L"н", L"п", L"в");
    chatConversationPage.addOutputs(chat::send_message, chat::chat_options, chat::public_chat, chat::back);

    do
    {
        if (pm_user->isBanned())
        {
            pgDefault();
            pm_user = nullptr;
            return chat::user_banned;
        }
        if (pm_user->getUserLogin() == L"complaint_bot")
        {
            pgDefault();
            pm_user = nullptr;
            return chat::user_is_service;
        }
        auto messages = db->getAllPrivateMessagesForUsersById(user->getId(), pm_user->getId(), pg_MaxItems);
        messagesList(std::move(messages));
        if (pg_MaxItems <= 0)
        {
            std::wcout << L"В этом чате нет сообщений. Начните общение первым." << std::endl;
        }

        chatDescription = L"Личные сообщения. Беседа с пользователем.\n"
                          L"Вы:\t" +
                          user->getUserName() + L" [" + user->getUserLogin() + L"]\t[userID " + std::to_wstring(user->getId()) + L"]\n" +
                          L"Ваш собеседник:\t" + pm_user->getUserName() + L" [" + pm_user->getUserLogin() + L"]\t[userID " + std::to_wstring(pm_user->getId()) + L"]\n" +
                          std::wstring((user->getId() == pm_user->getId()) ? L"Вы ведете личную беседу с самим собой.\n" : std::wstring()) +
                          L"Показаны сообщения: " +
                          std::to_wstring((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + L" - " +
                          std::to_wstring(pg_EndItem) + L" из " +
                          std::to_wstring(pg_MaxItems);
        chatConversationPage.setDescription(chatDescription);
        result = chatConversationPage.IOgetline();
        switch (result)
        {
        case chat::send_message:
            sendMessage(chat::private_chat);
            break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::public_chat:
            result = chat::public_chat;
            break;
        case chat::back:
            result = chat::empty;
            break;
        default:
            break;
        }
        system(clear);
    } while ((result != chat::public_chat) && (result != chat::empty));
    pgDefault();
    pm_user = nullptr;
    return result;
}

void ChatUserInterface::complaint()
{
    UserInput<std::wstring, std::wstring> complaintTextIO(std::wstring(), L"Укажите причину жалобы: ", std::wstring());
    UserInput<int, int> troubleMsgIdIO(std::wstring(), L"Укажите messageID сообщения на которое хотите пожаловаться: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), L"Отменить жалобу? (да - отменить / нет - не отменять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes, chat::no, chat::yes, chat::no);

    std::wstring text = complaintTextIO.IOgetlineThrough(true);
    std::unique_ptr<Message> troubleMsg = nullptr;

    do
    {
        int troubleMsgId = troubleMsgIdIO.IOcinThrough();
        auto _troubleMsg = db->getMessage(troubleMsgId);
        if (_troubleMsg == nullptr)
        {
            std::wcout << L"Указан неверный messageID" << std::endl;
            chat::Results yesno = yesnoIO.IOgetline();
            if (yesno == chat::yes)
            {
                return;
            }
            else if (yesno == chat::no)
            {
                continue;
            }
        }
        else
        {
            troubleMsg = std::move(_troubleMsg);
        }
    } while (troubleMsg == nullptr);

    auto troubleUser = db->getUserById(troubleMsg->getAuthorID());
    std::wstring messageText = L"\nЖалоба от пользователя: " +
                              user->getUserName() +
                              L"[" + user->getUserLogin() + L"][userId " + std::to_wstring(user->getId()) + L"]\n" +
                              L"На пользователя: " +
                              troubleUser->getUserName() +
                              L"[" + troubleUser->getUserLogin() + L"][userId " + std::to_wstring(troubleUser->getId()) + L"]\n" +
                              L"Текст жалобы: " + text +
                              L"\nЗа сообщение: [messageId " + std::to_wstring(troubleMsg->getId()) + L"]. Текст сообщения: " + troubleMsg->getMessage() +
                              L"\nuserID автора сообщения: " + std::to_wstring(troubleMsg->getAuthorID());

    yesnoIO.setDescription(messageText);
    yesnoIO.setMainMessage(L"Вы действительно хотите отправить эту жалобу администрации чата? (да / нет): ");
    yesnoIO.IOgetline();
    Message m;
    m.setAuthorID(user->getId());
    m.setRecipientID(2);
    m.setMessage(messageText);
    db->addMessage(m);
}