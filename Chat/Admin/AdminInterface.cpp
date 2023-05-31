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
    UserInput<std::string, chat::Results> adminAreaPage(
        "Раздел администратора.",
        "Опции: "
        "\nп - управление пользователями;"
        "\nж - рассмотрение жалоб;"
        "\nв - выход;"
        "\nУкажите опцию: ",
        "Неверный ввод",
        3);

    adminAreaPage.addInputs("п", "ж", "в");
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
    std::string chatDescription = "Управление пользователями";
    std::string chatMainMessage = "Опции: "
                                  "\nб - заблокировать;"
                                  "\nр - разблокировать;"
                                  "\nа - сделать администратором;"
                                  "\nна - снять администратора;"
                                  "\nнс - навигация по списку пользователей;"
                                  "\nн - назад;"
                                  "\nУкажите опцию: ";
    UserInput<std::string, chat::Results> usersPage(chatDescription, chatMainMessage, "Неверный ввод", 6);
    usersPage.addInputs("б", "р", "а", "на", "нс", "н");
    usersPage.addOutputs(chat::admin_ban_user,
                         chat::admin_unban_user,
                         chat::user_to_admin,
                         chat::admin_to_user,
                         chat::in_chat_navigation,
                         chat::in_admin_area);
    UserInput<std::string, chat::Results> usersEmptyPage("В чате нет пользователей", "н - назад: ", "Неверный ввод", 1);
    usersEmptyPage.addInputs("н");
    usersEmptyPage.addOutputs(chat::in_admin_area);

    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;
    chat::Results userInputResult = chat::user_input;
    do
    {
        std::cout << "Вы находитесь на странице Управления пользователями." << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 пользователей
            auto u = usersDB.listLast(start, perPage, end);
            printList(u, "В этом чате нет пользователей.", "Пользователь №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного пользователя
            auto u = usersDB.list(start, perPage, end);
            printList(u, "В этом чате нет пользователей.", "Пользователь №", start);
        }

        if (!usersDB.empty())
        {
            std::cout << "Показаны пользователи " << start + 1 << " - " << end << " из " << usersDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < usersDB.getCount())
        {
            std::cout << "Внимание. Вы не увидите последних пользователей, они находятся за диапазоном отображения." << std::endl;
        }
        if (userInputResult == chat::user_input && !usersDB.empty())
            userInputResult = usersPage.IOgetline();
        if (userInputResult == chat::user_input && usersDB.empty())
            userInputResult = usersEmptyPage.IOgetline();
        switch (userInputResult)
        {
        case chat::admin_ban_user:
            userInputResult = setUserStatus("Заблокировать пользователя.", userInputResult);
            break;
        case chat::admin_unban_user:
            userInputResult = setUserStatus("Разблокировать пользователя.", userInputResult);
            break;
        case chat::user_to_admin:
            userInputResult = setUserStatus("Сделать пользователя администратором.", userInputResult);
            break;
        case chat::admin_to_user:
            userInputResult = setUserStatus("Снять статус администратора.", userInputResult);
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

chat::Results AdminInterface::setUserStatus(std::string description, chat::Results status)
{
    UserInput<int, int> getUser(description, "Укажите userid: ", std::string());
    UserInput<std::string, chat::Results> yesnoIO(std::string(), "Отменить операцию? (да - отменить / нет - не отменять): ", "Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs("да", "нет", "yes", "no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);
    std::shared_ptr<User> user = nullptr;
    chat::Results userInputResult = chat::user_input;
    do
    {
        int userId = 0;
        bool fail = false;
        std::string msg;
        userId = getUser.IOcinThrough();
        user = usersDB.getUserByID(userId);

        if (user == nullptr)
        {
            msg = "Указан неверный userid";
            fail = true;
        }
        if (user != nullptr)
        {
            if (AuthorizedUser->getID() == user->getID())
            {
                msg = "Вы не можете проводить операции над самим собой. Это может сделать другой администратор.";
                fail = true;
            }
            if (user->getID() == 0)
            {
                msg = "Нельзя менять статус сервисному администратору.";
                fail = true;
            }
        }
        if (fail)
        {
            user = nullptr;
            std::cout << msg << std::endl;
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
    std::string chatMainMessage = "Опции: "
                                  "\nуб - удалить сообщение, заблокировать автора;"
                                  "\nунб - удалить сообщение, не блокировать автора;"
                                  "\nр - пометить решенной, санкции не применяются;"
                                  "\nу - удалить жалобу, санкции не применяются;"
                                  "\nнс - навигация по списку жалоб;"
                                  "\nн - назад;"
                                  "\nУкажите опцию: ";
    UserInput<std::string, chat::Results> complaintPage("Управление жалобами.", chatMainMessage, "Неверный ввод", 6);
    complaintPage.addInputs("уб", "унб", "р", "у", "нс", "н");
    complaintPage.addOutputs(chat::complaint_delete_message_ban,
                             chat::complaint_delete_message_noban,
                             chat::complaint_complete,
                             chat::complaint_delete,
                             chat::in_chat_navigation,
                             chat::in_admin_area);
    UserInput<std::string, chat::Results> complaintEmptyPage("Жалобы отсутствуют.", "н - назад: ", "Неверный ввод", 1);
    complaintEmptyPage.addInputs("н");
    complaintEmptyPage.addOutputs(chat::in_admin_area);

    uint start = 0, perPage = 10, end = 0, max = 0;
    chat::Pagination listMode = chat::Pagination::last_page;
    chat::Results userInputResult = chat::user_input;

    do
    {
        std::cout << "Вы находитесь на странице рассмотрения жалоб.\n"
                  << std::endl;
        if (listMode == chat::Pagination::last_page)
        {
            // последние 10 сообщений
            auto m = complaintsDB.listLast(start, perPage, end);
            printList(m, "В этом чате нет жалоб.", "Жалоба №", start);
        }

        if (listMode == chat::Pagination::element_)
        {
            // список от указанного сообщения
            auto m = complaintsDB.list(start, perPage, end);
            printList(m, "В этом чате нет жалоб.", "Жалоба №", start);
        }

        if (!complaintsDB.empty())
        {
            std::cout << "Показаны жалобы " << start + 1 << " - " << end << " из " << complaintsDB.getCount() << std::endl;
        }
        if (listMode == chat::Pagination::element_ && start + perPage < complaintsDB.getCount())
        {
            std::cout << "Внимание. Вы не увидите последние жалобы, они находятся за диапазоном отображения." << std::endl;
        }
        std::cout << std::endl;

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
    UserInput<int, int> get("Управление жалобой", "Укажите complaint id: ", std::string());
    UserInput<std::string, chat::Results> yesnoIO(std::string(), "Отменить операцию? (да - отменить / нет - не отменять): ", "Неверный ввод. Требуется да или нет", 4);
    yesnoIO.addInputs("да", "нет", "yes", "no");
    yesnoIO.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);
    std::shared_ptr<Complaint> complaint = nullptr;
    do
    {
        int complaintID = get.IOcinThrough();
        complaint = complaintsDB.getComplaintByID(complaintID);

        if (complaint == nullptr)
        {
            std::cout << "Жалобы с таким ID не существует" << std::endl;
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
