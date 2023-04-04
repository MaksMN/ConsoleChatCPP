#include "ChatAdminInterface.h"

chat::Results ChatAdminInterface::run(std::unique_ptr<DB> _db)
{
    db = std::move(_db);
    auto complaintBotTemp = db->getUserByLogin(L"complaint_bot");
    this->complaintBot = std::move(complaintBotTemp);
    auto _login = login();
    if (_login == chat::login_success && user->isAdmin())
    {
        mainPage();
    }
    else
    {
        std::wcout << L"У вас не получилось войти в раздел администратора." << std::endl;
        return chat::login_cancel;
    }
    return chat::empty;
}

void ChatAdminInterface::mainPage()
{
    system(clear);
    pgDefault();
    UserInput<std::wstring, chat::Results> adminAreaPage(
        L"Раздел администратора.",
        L"Опции: "
        L"\nп - управление пользователями;"
        L"\nс - управление сообщениями;"
        L"\nж - рассмотрение жалоб;"
        L"\nв - выход;"
        L"\nУкажите опцию: ",
        L"Неверный ввод",
        4);

    adminAreaPage.addInputs(L"п", L"с", L"ж", L"в");
    adminAreaPage.addOutputs(chat::user_list, chat::messages, chat::complaint, chat::back);

    do
    {
        system(clear);
        auto result = adminAreaPage.IOgetline();
        switch (result)
        {
        case chat::user_list:
            usersManage();
            break;
        case chat::messages:
            messagesManage();
            break;
        case chat::complaint:
            complaintManage();
            break;
        case chat::back:
            system(clear);
            pgDefault();
            return;
            break;
        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::complaintManage()
{
    system(clear);
    pgDefault();
    UserInput<std::wstring, chat::Results> complaintPage(std::wstring(), std::wstring(), L"Неверный ввод", 5);
    complaintPage.addInputs(L"у", L"нс", L"п", L"с", L"н");
    complaintPage.addOutputs(chat::delete_message,
                             chat::chat_options,
                             chat::user_list,
                             chat::messages,
                             chat::back);

    do
    {
        std::wstring chatDescription;
        std::wstring chatMainMessage;
        system(clear);
        auto complaintList = db->getAllPrivateMessagesForUserById(complaintBot->getId(), pg_MaxItems);
        messagesList(std::move(complaintList));
        if (pg_MaxItems <= 0)
        {
            std::wcout << L"Жалобы отсутствуют." << std::endl;
        }
        chatDescription = L"Список жалоб. Показаны жалобы: " + pgInfo();
        chatMainMessage = L"Опции: " +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nу - удалить жалобу;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nнс - навигация по списку жалоб;") +
                          L"\nп - управление пользователями;"
                          L"\nс - управление сообщениями;"
                          L"\nн - назад;"
                          L"\nУкажите опцию: ";
        complaintPage.setDescription(chatDescription);
        complaintPage.setMainMessage(chatMainMessage);
        auto result = complaintPage.IOgetline();
        switch (result)
        {
        case chat::delete_message:
        {
            if (pg_MaxItems > 0)
            {
                deleteMessage(true);
            }
        }
        break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::user_list:
            usersManage();
            break;
        case chat::messages:
            messagesManage();
            break;
        case chat::back:
            system(clear);
            pgDefault();
            return;
        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::messagesManage()
{
    system(clear);
    pgDefault();
    UserInput<std::wstring, chat::Results> messagesPage(std::wstring(), std::wstring(), L"Неверный ввод", 3);
    messagesPage.addInputs(L"у", L"нс", L"н");
    messagesPage.addOutputs(chat::delete_message,
                            chat::chat_options,
                            chat::back);
    do
    {
        std::wstring chatDescription;
        std::wstring chatMainMessage;
        system(clear);
        auto messages = db->getAllPublicMessages(pg_MaxItems);
        messagesList(std::move(messages));
        if (pg_MaxItems <= 0)
        {
            std::wcout << L"Публичные сообщения отсутствуют." << std::endl;
        }
        chatDescription = L"Список сообщений. Показаны сообщения: " + pgInfo();
        chatMainMessage = L"Опции: " +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nу - удалить сообщение;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nнс - навигация по списку сообщений;") +
                          L"\nн - назад;"
                          L"\nУкажите опцию: ";
        messagesPage.setDescription(chatDescription);
        messagesPage.setMainMessage(chatMainMessage);
        auto result = messagesPage.IOgetline();
        switch (result)
        {
        case chat::delete_message:
        {
            if (pg_MaxItems > 0)
            {
                deleteMessage();
            }
        }
        break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::back:
            system(clear);
            pgDefault();
            return;
        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::usersManage()
{
    system(clear);
    pgDefault();
    UserInput<std::wstring, chat::Results> usersPage(std::wstring(), std::wstring(), L"Неверный ввод", 6);
    usersPage.addInputs(L"б", L"р", L"а", L"на", L"нс", L"н");
    usersPage.addOutputs(chat::user_ban,
                         chat::user_unban,
                         chat::user_admin,
                         chat::user_not_admin,
                         chat::chat_options,
                         chat::back);

    do
    {
        std::wstring chatDescription;
        std::wstring chatMainMessage;
        system(clear);
        auto users = db->getAllUsers();
        pg_MaxItems = db->usersCount();
        usersList(std::move(users));

        if (pg_MaxItems <= 0)
        {
            std::wcout << L"В чате нет пользователей." << std::endl;
        }
        chatDescription = L"Список пользователей. Показаны пользователи: " + pgInfo();
        chatMainMessage = L"Опции: " +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nб - заблокировать;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nр - разблокировать;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nа - сделать администратором;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nна - снять администратора;") +
                          std::wstring((pg_MaxItems <= 0) ? std::wstring() : L"\nнс - навигация по списку пользователей;") +
                          L"\nн - назад;"
                          L"\nУкажите опцию: ";
        usersPage.setDescription(chatDescription);
        usersPage.setMainMessage(chatMainMessage);
        auto result = usersPage.IOgetline();
        switch (result)
        {
        case chat::user_ban:
        {
            if (pg_MaxItems > 0)
            {
                userBan(true);
            }
        }
        break;
        case chat::user_unban:
        {
            if (pg_MaxItems > 0)
            {
                userBan(false);
            }
        }
        break;
        case chat::user_admin:
        {
            if (pg_MaxItems > 0)
            {
                userSetAdmin(true);
            }
        }
        break;
        case chat::user_not_admin:
        {
            if (pg_MaxItems > 0)
            {
                userSetAdmin(false);
            }
        }
        break;
        case chat::chat_options:
            pgNavigation();
            break;
        case chat::back:
            system(clear);
            pgDefault();
            return;
        default:
            break;
        }
    } while (1);
}

void ChatAdminInterface::deleteMessage(bool complaint_only)
{
    UserInput<int, int> getMessage(std::wstring(), L"Укажите messageId: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), L"Вы действительно хотите удалить сообщение? (да - удалить / нет - не удалять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes, chat::no, chat::yes, chat::no);

    do
    {
        int msgId = getMessage.IOcinThrough();
        auto message = db->getMessage(msgId);
        if (message != nullptr)
        {
            if (complaint_only && (message->getRecipientID() != 2))
            {
                std::wcout << L"Сообщение не является жалобой." << std::endl;
                continue;
            }
            if (!complaint_only && message->isPrivate)
            {
                std::wcout << L"Указан неверный messageId" << std::endl;
                continue;
            }
            auto result = yesnoIO.IOgetline();
            if (result == chat::yes)
            {
                db->deleteMessageById(msgId);
                return;
            }
            else
            {
                return;
            }
        }
        std::wcout << L"Указан неверный messageId" << std::endl;
    } while (true);
}

void ChatAdminInterface::userBan(bool ban)
{
    UserInput<int, int> getUser(std::wstring(), L"Укажите userId: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), std::wstring(), L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes, chat::no, chat::yes, chat::no);

    do
    {
        int userId = 0;
        userId = getUser.IOcinThrough();
        auto _user = db->getUserById(userId);
        if (_user != nullptr)
        {
            if ((user->getId() == _user->getId()) || (_user->getId() == 2) || (_user->getId() == 1))
            {
                std::wcout << L"Операция недопустима для себя или сервисных админа или бота." << std::endl;
                yesnoIO.setMainMessage(L"Выйти из меню блокировки пользователей? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {
                    return;
                }
                continue;
            }
            if ((_user->isBanned() == ban) && ban)
            {
                std::wcout << L"Пользователь уже заблокирован." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {
                    return;
                }
                continue;
            }
            if ((_user->isBanned() == ban) && !ban)
            {
                std::wcout << L"Пользователь не заблокирован." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {
                    return;
                }
                continue;
            }
            if (ban)
            {
                yesnoIO.setMainMessage(L"Вы действительно хотите заблокировать пользователя? (да - заблокировать / нет - не блокировать): ");
            }
            else
            {
                yesnoIO.setMainMessage(L"Вы действительно хотите разблокировать пользователя? (да - разблокировать / нет - оставить заблокированным): ");
            }
            auto result = yesnoIO.IOgetline();
            if (result == chat::yes)
            {
                _user->setIsBanned(ban);
                if (ban)
                {
                    _user->setIsAdmin(false);
                }
                db->updateUserData(*_user);
                return;
            }
            else
            {
                return;
            }
        }
        std::wcout << L"Указан неверный userId" << std::endl;
    } while (true);
}

void ChatAdminInterface::userSetAdmin(bool adm)
{
    UserInput<int, int> getUser(std::wstring(), L"Укажите userId: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), std::wstring(), L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes, chat::no, chat::yes, chat::no);
    auto result = chat::empty;
    do
    {
        int userId = 0;
        userId = getUser.IOcinThrough();
        auto _user = db->getUserById(userId);
        if (_user != nullptr)
        {
            if ((user->getId() == _user->getId()) || (_user->getId() == 2) || (_user->getId() == 1))
            {
                std::wcout << L"Нельзя менять статус администратора самого себя или сервисных админа или бота." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {

                    return;
                }
                continue;
            }
            if (_user->isBanned())
            {
                std::wcout << L"Этот пользователь заблокирован. Его нельзя сделать администратором." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {

                    return;
                }
                continue;
            }
            if ((_user->isAdmin() == adm) && adm)
            {
                std::wcout << L"Пользователь уже администратор." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {

                    return;
                }
                continue;
            }
            if ((_user->isAdmin() == adm) && !adm)
            {
                std::wcout << L"Пользователь не является администратором." << std::endl;
                yesnoIO.setMainMessage(L"Выйти? (да - выйти / нет - не выходить): ");
                if (yesnoIO.IOgetline() == chat::yes)
                {
                    return;
                }
                continue;
            }
            if (adm)
            {
                yesnoIO.setMainMessage(L"Вы действительно хотите дать права администратора? ( да / нет ): ");
            }
            else
            {
                yesnoIO.setMainMessage(L"Вы действительно хотите снять права администратора? ( да / нет ): ");
            }
            result = yesnoIO.IOgetline();
            if (result == chat::yes)
            {
                _user->setIsAdmin(adm);
                db->updateUserData(*_user);

                return;
            }
            else
            {
                return;
            }
        }
        std::wcout << L"Указан неверный userId" << std::endl;
    } while (true);
}