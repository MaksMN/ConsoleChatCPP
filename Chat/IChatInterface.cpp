#include "IChatInterface.h"

IChatInterface::IChatInterface(DBmessages &_pubMessagesDB,
                               DBmessages &_privMessagesDB,
                               DBcomplaints &_complaintsDB,
                               DBusers &_usersDB)
    : pubMessagesDB(_pubMessagesDB),
      privMessagesDB(_privMessagesDB),
      complaintsDB(_complaintsDB),
      usersDB(_usersDB) {}

chat::Results IChatInterface::login()
{
    AuthorizedUser = nullptr;
    std::wstring incorrectInput = L"Неверный ввод. Пустые значения недопустимы.";

    UserInput<std::wstring, std::wstring> getLogin(L"Страница входа", L"Введите логин: ", incorrectInput);
    UserInput<std::wstring, std::wstring> getPass(std::wstring(), L"Введите пароль: ", incorrectInput);
    UserInput<std::wstring, chat::Results> loginCancel(std::wstring(), L"Отменить вход? (да - отменить, нет - продолжить): ", L"Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs(L"да", L"нет", L"yes", L"no");
    loginCancel.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    // ввод логина
    do
    {
        std::wstring login = getLogin.IOgetlineThrough(true);

        AuthorizedUser = usersDB.getUserByLogin(login);

        if (AuthorizedUser != nullptr)
        {
            break;
        }
        else
        {
            std::wcout << L"Пользователь с таким логином не существует" << std::endl;
        }

        chat::Results endInput = loginCancel.IOgetline();
        if (endInput == chat::yes_)
        {
            AuthorizedUser = nullptr;
            return chat::login_cancel;
        }

    } while (1);

    // ввод пароля
    bool validate = false;
    do
    {
        std::wstring password = getPass.IOgetlineThrough(true, false);
        validate = AuthorizedUser->validatePass(password);
        if (!validate)
        {
            std::wcout << L"Неверный пароль: " << std::endl;
            chat::Results endInput = loginCancel.IOgetline();
            if (endInput == chat::yes_)
            {
                AuthorizedUser = nullptr;
                return chat::login_cancel;
            }
        }
    } while (!validate);

    if (AuthorizedUser->isBanned())
        return chat::user_banned;

    return chat::login_success;
}

void IChatInterface::printMessages(std::vector<std::shared_ptr<Message>> &messages, const std::wstring &&emptyMsg, const std::wstring &&beforeMsg, const uint &start) const
{
    if (messages.empty())
    {
        std::wcout << emptyMsg << std::endl;
        return;
    }
    uint k = start;
    for (auto &i : messages)
    {
        std::wcout << beforeMsg << L"[" << ++k << L"]" << std::endl;
        auto user = usersDB.getUserByID(i->getAuthorID());
        user->printData();
        i->printData();
    }
}

chat::Results IChatInterface::listNavigation(uint &start, uint &perPage, uint &end, chat::Pagination &mode, uint max)
{
    UserInput<std::wstring, chat::Pagination> selectOption(std::wstring(),
                                                          L"Выберите опцию:"
                                                          L"\nэнс - элементов на странице;"
                                                          L"\nпкэ - перейти к элементу №...;"
                                                          L"\nсбр - сброс настроек (всегда последние 10 элементов);"
                                                          L"\nн - вернуться в чат;"
                                                          L"\nВведите значение: ",
                                                          L"Неверный ввод.", 4);
    selectOption.addInputs(L"энс", L"пкэ", L"сбр", L"н");
    selectOption.addOutputs(chat::Pagination::msg_per_page, chat::Pagination::select_message, chat::Pagination::select_last_page, chat::Pagination::in_chat);

    UserInput<int, int> getInt(std::wstring(), std::wstring(), L"Неверный ввод");

    switch (selectOption.IOgetline())
    {
    case chat::Pagination::msg_per_page:
        getInt.setMainMessage(L"Укажите количество элементов на странице: ");
        perPage = getInt.IOcinThrough();
        if (perPage < 1)
        {
            perPage = 1;
        }
        break;
    case chat::Pagination::select_message:
        getInt.setMainMessage(L"Укажите номер элемента (1 - " + std::to_wstring(max) + L"): ");
        start = getInt.IOcinThrough() - 1;
        mode = chat::Pagination::element_;
        break;
    case chat::Pagination::select_last_page:
        mode = chat::Pagination::last_page;
        start = 0;
        perPage = 10;
        break;
    default:
        break;
    }
    system(clear);
    return chat::user_input;
}
