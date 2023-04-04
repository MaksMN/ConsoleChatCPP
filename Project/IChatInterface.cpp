#include "IChatInterface.h"

chat::Results IChatInterface::login()
{
    chat::Results endInput = chat::empty;
    std::wstring incorrectInput = L"Неверный ввод. Пустые значения недопустимы.";

    UserInput<std::wstring, std::wstring> getLogin(L"Страница входа", L"Введите логин: ", incorrectInput);
    UserInput<std::wstring, std::wstring> getPass(std::wstring(), L"Введите пароль: ", incorrectInput);
    UserInput<std::wstring, chat::Results> loginCancel(std::wstring(), L"Отменить вход? (да - отменить, нет - продолжить): ", L"Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs(L"да", L"нет", L"yes", L"no");
    loginCancel.addOutputs(chat::yes, chat::no, chat::yes, chat::no);

    // ввод логина
    do
    {
        std::wstring _login = getLogin.IOgetlineThrough(true);

        try
        {
            std::unique_ptr<User> _user = db->getUserByLogin(_login, true);
            user = std::move(_user);
            break;
        }
        catch (UserNotFoundException &e)
        {
            std::wcout << e.what() << std::endl;
            endInput = loginCancel.IOgetline();
            if (endInput == chat::yes)
            {
                return chat::login_cancel;
            }
        }
    } while (1);

    // ввод пароля
    bool validate = false;
    do
    {
        std::wstring password = getPass.IOgetlineThrough(true);
        validate = db->isCorrectPassword(user->getId(), password);
        if (!validate)
        {
            std::wcout << L"Неверный пароль: " << std::endl;
            endInput = loginCancel.IOgetline();
            if (endInput == chat::yes)
            {
                return chat::login_cancel;
            }
        }
    } while (!validate);

    if (user->isBanned())
    {
        return chat::user_banned;
    }
    return chat::login_success;
}

void IChatInterface::pagination()
{
    if (pg_StartItem < 0)
    {
        pg_StartItem = 0;
    }
    if (pg_pageNumber < 1)
    {
        pg_pageNumber = 1;
    }
    // Если пустой массив
    if (pg_MaxItems == 0)
    {
        pg_StartItem = 0;
        pg_EndItem = 0;
        return;
    }

    // количество сообщений на страницу не должно превышать максимального количества сообщений
    if (pg_itemsPerPage >= pg_MaxItems)
    {
        pg_StartItem = 0;
        pg_EndItem = pg_MaxItems;
        return;
    }

    if (pg_itemsPerPage < 1)
    {
        pg_itemsPerPage = 1;
    }

    // максимально возможный номер страницы, урезается если введен превышающий диапазон
    pg_maxPageNumber = (pg_MaxItems / pg_itemsPerPage) + 1;
    if (pg_pageNumber > pg_maxPageNumber)
    {
        pg_pageNumber = pg_maxPageNumber;
    }

    // если запрошена страница
    if (paginationMode == page::page)
    {
        pg_StartItem = pg_itemsPerPage * (pg_pageNumber - 1);
        pg_EndItem = pg_StartItem + pg_itemsPerPage;
    }

    // если запрошен показ страницы от конкретного сообщения
    if (paginationMode == page::message)
    {
        pg_EndItem = pg_StartItem + pg_itemsPerPage;
    }

    // pg_StartItem + pg_itemsPerPage превысил максимум или запрошена последняя страница
    // будут отображаться последние pg_itemsPerPage сообщений
    if ((pg_EndItem > pg_MaxItems) || (paginationMode == page::last_page))
    {
        pg_StartItem = pg_MaxItems - pg_itemsPerPage;
        pg_EndItem = pg_MaxItems;
    }
}

void IChatInterface::pgDefault()
{
    paginationMode = page::last_page;
    pg_pageNumber = 1;
    pg_MaxItems = 0;
    pg_itemsPerPage = 10;
    pg_maxPageNumber = 0;
    pg_StartItem = 0;
    pg_EndItem = 0;
}

void IChatInterface::pgNavigation()
{
    UserInput<std::wstring, page::PaginationMode> selectOption(std::wstring(),
                                                              L"Выберите опцию:"
                                                              L"\nэнс - элементов на странице;"
                                                              L"\nпнс - перейти на страницу...;"
                                                              L"\nпкэ - перейти к элементу №...;"
                                                              L"\nсбр - сброс настроек (всегда последние 10 элементов);"
                                                              L"\nн - вернуться в чат;"
                                                              L"\nВведите значение: ",
                                                              L"Неверный ввод.", 5);
    selectOption.addInputs(L"энс", L"пнс", L"пкэ", L"сбр", L"н");
    selectOption.addOutputs(page::msg_per_page, page::page, page::message, page::last_page, page::close_options);

    UserInput<int, int> getInt(std::wstring(), std::wstring(), L"Неверный ввод");

    switch (selectOption.IOgetline())
    {
    case page::msg_per_page:
        getInt.setMainMessage(L"Укажите количество элементов на странице (1 - " + std::to_wstring(pg_MaxItems) + L"): ");
        pg_itemsPerPage = getInt.IOcinThrough();
        break;
    case page::page:
        getInt.setMainMessage(L"Укажите номер страницы (1 - " + std::to_wstring(pg_maxPageNumber) + L"): ");
        pg_pageNumber = getInt.IOcinThrough();
        paginationMode = page::page;
        break;
    case page::message:
        getInt.setMainMessage(L"Укажите номер элемента (1 - " + std::to_wstring(pg_MaxItems) + L"): ");
        pg_StartItem = getInt.IOcinThrough() - 1;
        paginationMode = page::message;
        break;
    case page::last_page:
        pgDefault();
    default:
        break;
    }
}

std::wstring IChatInterface::pgInfo()
{
    return std::to_wstring((pg_MaxItems <= 0) ? pg_StartItem : pg_StartItem + 1) + L" - " +
           std::to_wstring(pg_EndItem) + L" из " +
           std::to_wstring(pg_MaxItems);
}

std::wstring IChatInterface::StampToTime(long long timestamp)
{
    time_t tick = (time_t)(timestamp);
    struct tm tm;
    char s[40];
    tm = *localtime(&tick);
    std::wstring day = (tm.tm_mday < 10) ? L"0" + std::to_wstring(tm.tm_mday) : std::to_wstring(tm.tm_mday);
    std::wstring month = (tm.tm_mon < 10) ? L"0" + std::to_wstring(tm.tm_mon) : std::to_wstring(tm.tm_mon);
    std::wstring year = std::to_wstring(tm.tm_year + 1900);
    std::wstring hour = (tm.tm_hour < 10) ? L"0" + std::to_wstring(tm.tm_hour) : std::to_wstring(tm.tm_hour);
    std::wstring min = (tm.tm_min < 10) ? L"0" + std::to_wstring(tm.tm_min) : std::to_wstring(tm.tm_min);
    std::wstring sec = (tm.tm_sec < 10) ? L"0" + std::to_wstring(tm.tm_sec) : std::to_wstring(tm.tm_sec);
    return std::wstring(day + L"." + month + L"." + year + L" " + hour + L":" + min + L":" + sec);
}

void IChatInterface::messagesList(std::unique_ptr<Message[]> messages)
{
    pagination();
    for (int i{pg_StartItem}; i < pg_EndItem && messages != nullptr; i++)
    {
        auto msgUser = db->getUserById(messages[i].getAuthorID());

        std::wcout << std::endl;
        std::wcout
            << i + 1 << L". "
            << StampToTime(messages[i].getDate()) + L" "
            << msgUser->getUserName()
            << L"[" << msgUser->getUserLogin() << L"]"
            << L"[userID " << std::to_wstring(msgUser->getId()) << L"]"
            << (msgUser->isBanned() ? L"[banned]" : std::wstring())
            << (msgUser->isAdmin() ? L"[admin]" : std::wstring())
            << L" [messageID " << messages[i].getId() << L"] "
            << std::endl;
        std::wcout << messages[i].getMessage() << std::endl;
    }
    std::wcout << std::endl;
}

void IChatInterface::usersList(std::unique_ptr<User[]> users)
{
    pg_MaxItems = db->usersCount();
    pagination();
    for (int i{pg_StartItem}; i < pg_EndItem && users != nullptr; i++)
    {
        std::wcout
            << i + 1 << L". "
            << users[i].getUserName()
            << L"[" << users[i].getUserLogin() << L"]"
            << L"[userID " << std::to_wstring(users[i].getId()) << L"]"
            << (users[i].isBanned() ? L"[banned]" : std::wstring())
            << (users[i].isAdmin() ? L"[admin]" : std::wstring())
            << std::endl;
        std::wcout << std::endl;
    }
    std::wcout << std::endl;
}

void IChatInterface::userProfile()
{
    UserInput<std::wstring, user::options> options(L"Настройки пользователя. " + user->getUserName() + L"[" + user->getUserLogin() + L"]",
                                                  L"Какие данные вы хотите поменять? (л - логин; п - пароль; и - имя; з - закончить): ",
                                                  L"Неверный ввод",
                                                  4);
    options.addInputs(L"л", L"п", L"и", L"з");
    options.addOutputs(user::login, user::pass, user::name, user::end);

    UserInput<std::wstring, std::wstring> getLogin(L"Изменение логина", L"Введите логин: ", std::wstring());
    UserInput<std::wstring, std::wstring> getName(L"Изменение имени", L"Введите имя: ", std::wstring());
    UserInput<std::wstring, std::wstring> getPass(L"Изменение пароля", L"Введите пароль: ", std::wstring());
    UserInput<std::wstring, chat::Results> loginCancel(std::wstring(), L"Отменить смену логина? (да - отменить, нет - продолжить): ", L"Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs(L"да", L"нет", L"yes", L"no");
    loginCancel.addOutputs(chat::yes, chat::no, chat::yes, chat::no);

    std::wstring loginTemp;
    std::wstring name;
    std::wstring pass;

    do
    {
        options.setDescription(L"Настройки пользователя. " + user->getUserName() + L"[" + user->getUserLogin() + L"]");
        user::options result = options.IOgetline();
        switch (result)
        {
        case user::login:
        {
            // ввод логина
            bool validLogin = false;
            do
            {
                loginTemp = getLogin.IOgetlineThrough(true);

                validLogin = db->isUniqueLogin(loginTemp);
                if (!validLogin)
                {
                    std::wcout << L"Этот логин занят!" << std::endl;
                    if (loginCancel.IOgetline() == chat::yes)
                    {
                        break;
                    }
                }
                user->setUserLogin(loginTemp);
                db->updateUserData(*user);

            } while (!validLogin);
        }
        break;
        case user::name:
            name = getName.IOgetlineThrough(true);
            user->setUserName(name);
            db->updateUserData(*user);
            break;

        case user::pass:
            pass = getPass.IOgetlineThrough(true);
            db->setUserPassword(user->getId(), pass);
            break;
        case user::end:
            return;
        default:
            break;
        }

    } while (1);
}