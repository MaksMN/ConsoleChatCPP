#include "AdminInterface.h"

AdminInterface::AdminInterface(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB)
    : IChatInterface(_pubMessagesDB, _privMessagesDB, _complaintsDB, _usersDB) {}

chat::Results AdminInterface::run()
{
    auto _login = login();
    if (_login == chat::login_success && AuthorizedUser->isAdmin())
    {
        system(clear);
        mainPage();
    }
    else
    {
        system(clear);
        return chat::admin_login_fail;
    }
    return chat::in_chat_area;
}

chat::Results AdminInterface::mainPage()
{
    system(clear);
    UserInput<std::wstring, chat::Results> adminAreaPage(
        L"Раздел администратора.",
        L"Опции: "
        L"\nп - управление пользователями;"
        L"\nж - рассмотрение жалоб;"
        L"\nв - выход;"
        L"\nУкажите опцию: ",
        L"Неверный ввод",
        3);

    adminAreaPage.addInputs(L"п", L"ж", L"в");
    adminAreaPage.addOutputs(chat::admin_users_manage, chat::admin_complaint_manage, chat::in_chat_area);
    chat::Results userInputResult = chat::user_input;

    do
    {
        system(clear);
        userInputResult = adminAreaPage.IOgetline();
        switch (userInputResult)
        {
        case chat::admin_users_manage:
            usersManage();
            break;
        case chat::admin_complaint_manage:
            complaintsManage();
            break;
        default:
            break;
        }
    } while (userInputResult != chat::in_chat_area);
    return userInputResult;
}

chat::Results AdminInterface::usersManage()
{
    std::wstring chatDescription = L"Управление пользователями";
    std::wstring chatMainMessage = L"Опции: "
                                  L"\nб - заблокировать;"
                                  L"\nр - разблокировать;"
                                  L"\nа - сделать администратором;"
                                  L"\nна - снять администратора;"
                                  L"\nнс - навигация по списку пользователей;"
                                  L"\nн - назад;"
                                  L"\nУкажите опцию: ";
    UserInput<std::wstring, chat::Results> usersPage(chatDescription, chatMainMessage, L"Неверный ввод", 6);
    usersPage.addInputs(L"б", L"р", L"а", L"на", L"нс", L"н");
    usersPage.addOutputs(chat::admin_ban_user,
                         chat::admin_unban_user,
                         chat::user_to_admin,
                         chat::admin_to_user,
                         chat::in_chat_navigation,
                         chat::in_admin_area);
    UserInput<std::wstring, chat::Results> usersEmptyPage(L"В чате нет пользователей", L"н - назад: ", L"Неверный ввод", 1);
    usersEmptyPage.addInputs(L"н");
    usersEmptyPage.addOutputs(chat::in_admin_area);

    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;
    chat::Results userInputResult = chat::user_input;
    do
    {
        std::wcout << L"Вы находитесь на странице Управления пользователями." << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 пользователей
            auto u = usersDB.listLast(start, perPage, end);
            printList(u, L"В этом чате нет пользователей.", L"Пользователь №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного пользователя
            auto u = usersDB.list(start, perPage, end);
            printList(u, L"В этом чате нет пользователей.", L"Пользователь №", start);
        }

        if (!usersDB.empty())
        {
            std::wcout << L"Показаны пользователи " << start + 1 << L" - " << end << L" из " << usersDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < usersDB.getCount())
        {
            std::wcout << L"Внимание. Вы не увидите последних пользователей, они находятся за диапазоном отображения." << std::endl;
        }
        if (userInputResult == chat::user_input && !usersDB.empty())
            userInputResult = usersPage.IOgetline();
        if (userInputResult == chat::user_input && usersDB.empty())
            userInputResult = usersEmptyPage.IOgetline();
        switch (userInputResult)
        {
        case chat::admin_ban_user:
            userInputResult = setUserStatus(L"Заблокировать пользователя.", userInputResult);
            break;
        case chat::admin_unban_user:
            userInputResult = setUserStatus(L"Разблокировать пользователя.", userInputResult);
            break;
        case chat::user_to_admin:
            userInputResult = setUserStatus(L"Сделать пользователя администратором.", userInputResult);
            break;
        case chat::admin_to_user:
            userInputResult = setUserStatus(L"Снять статус администратора.", userInputResult);
            break;
        case chat::in_chat_navigation:
            userInputResult = listNavigation(start, perPage, end, listMode, usersDB.getCount());
            break;

        default:
            break;
        }
    } while (userInputResult != chat::in_admin_area);

    return chat::user_input;
}

chat::Results AdminInterface::setUserStatus(std::wstring description, chat::Results status)
{
    UserInput<int, int> getUser(description, L"Укажите userid: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), L"Отменить операцию? (да - отменить / нет - не отменять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);
    std::shared_ptr<User> user = nullptr;
    chat::Results userInputResult = chat::user_input;
    do
    {
        int userId = 0;
        bool fail = false;
        std::wstring msg;
        userId = getUser.IOcinThrough();
        user = usersDB.getUserByID(userId);

        if (user == nullptr)
        {
            msg = L"Указан неверный userid";
            fail = true;
        }
        if (user != nullptr)
        {
            if (AuthorizedUser->getID() == user->getID())
            {
                msg = L"Вы не можете проводить операции над самим собой. Это может сделать другой администратор.";                
                fail = true;
            }
            if (user->getID() == 0)
            {
                msg = L"Нельзя менять статус сервисному администратору.";                
                fail = true;
            }

        }
        if (fail)
        {
            user = nullptr;
            std::wcout << msg << std::endl;
            chat::Results yesno = yesnoIO.IOgetline();
            if (yesno == chat::yes_)
            {
                system(clear);
                return chat::user_input;
            }
            else if (yesno == chat::no_)
            {
                continue;
            }
        }
    } while (user == nullptr);
    if (status == chat::admin_ban_user)
        user->ban();
    if (status == chat::admin_unban_user)
        user->unBan();
    if (status == chat::user_to_admin)
        user->toAdmin();
    if (status == chat::admin_to_user)
        user->toUser();

    system(clear);
    return chat::user_input;
}

chat::Results AdminInterface::complaintsManage()
{
    std::wstring chatMainMessage = L"Опции: "
                                  L"\nуб - удалить сообщение, заблокировать автора;"
                                  L"\nунб - удалить сообщение, не блокировать автора;"
                                  L"\nр - пометить решенной, санкции не применяются;"
                                  L"\nу - удалить жалобу, санкции не применяются;"
                                  L"\nнс - навигация по списку жалоб;"
                                  L"\nн - назад;"
                                  L"\nУкажите опцию: ";
    UserInput<std::wstring, chat::Results> complaintPage(L"Управление жалобами.", chatMainMessage, L"Неверный ввод", 6);
    complaintPage.addInputs(L"уб", L"унб", L"р", L"у", L"нс", L"н");
    complaintPage.addOutputs(chat::complaint_delete_message_ban,
                             chat::complaint_delete_message_noban,
                             chat::complaint_complete,
                             chat::complaint_delete,
                             chat::in_chat_navigation,
                             chat::in_admin_area);
    UserInput<std::wstring, chat::Results> complaintEmptyPage(L"Жалобы отсутствуют.", L"н - назад: ", L"Неверный ввод", 1);
    complaintEmptyPage.addInputs(L"н");
    complaintEmptyPage.addOutputs(chat::in_admin_area);

    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;
    chat::Results userInputResult = chat::user_input;

    do
    {
        std::wcout << L"Вы находитесь на странице рассмотрения жалоб.\n"
                  << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 сообщений
            auto m = complaintsDB.listLast(start, perPage, end);
            printList(m, L"В этом чате нет жалоб.", L"Жалоба №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = complaintsDB.list(start, perPage, end);
            printList(m, L"В этом чате нет жалоб.", L"Жалоба №", start);
        }

        if (!complaintsDB.empty())
        {
            std::wcout << L"Показаны жалобы " << start + 1 << L" - " << end << L" из " << complaintsDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < complaintsDB.getCount())
        {
            std::wcout << L"Внимание. Вы не увидите последние жалобы, они находятся за диапазоном отображения." << std::endl;
        }
        std::wcout << std::endl;

        if (userInputResult == chat::user_input && !complaintsDB.empty())
        {
            userInputResult = complaintPage.IOgetline();
        }
        if (userInputResult == chat::user_input && complaintsDB.empty())
        {
            userInputResult = complaintEmptyPage.IOgetline();
        }
        if (userInputResult == chat::complaint_delete_message_ban ||
            userInputResult == chat::complaint_delete_message_noban ||
            userInputResult == chat::complaint_complete ||
            userInputResult == chat::complaint_delete)
        {
            userInputResult = complaintActions(userInputResult);
        }
        if (userInputResult == chat::in_chat_navigation)
            userInputResult = listNavigation(start, perPage, end, listMode, max);

    } while (userInputResult != chat::in_admin_area);

    return userInputResult;
}

chat::Results AdminInterface::complaintActions(chat::Results action)
{
    UserInput<int, int> get(L"Управление жалобой", L"Укажите complaint id: ", std::wstring());
    UserInput<std::wstring, chat::Results> yesnoIO(std::wstring(), L"Отменить операцию? (да - отменить / нет - не отменять): ", L"Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs(L"да", L"нет", L"yes", L"no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);
    std::shared_ptr<Complaint> complaint = nullptr;
    do
    {
        int complaintID = get.IOcinThrough();
        complaint = complaintsDB.getComplaintByID(complaintID);

        if (complaint == nullptr)
        {
            std::wcout << L"Жалобы с таким ID не существует" << std::endl;
            chat::Results yesno = yesnoIO.IOgetline();
            if (yesno == chat::yes_)
            {
                system(clear);
                return chat::in_admin_area;
            }
            else if (yesno == chat::no_)
            {
                continue;
            }
        }

    } while (complaint == nullptr);

    auto message = pubMessagesDB.getMessageByID(complaint->getTroubleMsgID());

    auto troubleUser = usersDB.getUserByID(complaint->getDefendantUID());

    switch (action)
    {
    case chat::complaint_delete_message_ban:
        pubMessagesDB.deleteMessage(complaint->getTroubleMsgID());
        complaint->complete();
        troubleUser->ban();
        break;
    case chat::complaint_delete_message_noban:
        pubMessagesDB.deleteMessage(complaint->getTroubleMsgID());
        complaint->complete();
        break;
    case chat::complaint_complete:
        complaint->complete();
        break;
    case chat::complaint_delete:
        complaintsDB.remove(complaint->getID());
        break;

    default:
        break;
    }
    return chat::user_input;
}
