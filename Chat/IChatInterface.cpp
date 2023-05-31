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
    std::string incorrectInput = "Неверный ввод. Пустые значения недопустимы.";

    UserInput<std::string, std::string> getLogin("Страница входа", "Введите логин: ", incorrectInput);
    UserInput<std::string, std::string> getPass(std::string(), "Введите пароль: ", incorrectInput);
    UserInput<std::string, chat::Results> loginCancel(std::string(), "Отменить вход? (да - отменить, нет - продолжить): ", "Неверный ввод. Требуется: да или нет", 4);
    loginCancel.addInputs("да", "нет", "yes", "no");
    loginCancel.addOutputs(chat::yes_, chat::no_, chat::yes_, chat::no_);

    // ввод логина
    do
    {
        std::string login = getLogin.IOgetlineThrough(true);

        AuthorizedUser = usersDB.getUserByLogin(login);

        if (AuthorizedUser != nullptr)
        {
            break;
        }
        else
        {
            std::cout << "Пользователь с таким логином не существует" << std::endl;
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
        std::string password = getPass.IOgetlineThrough(true);
        validate = AuthorizedUser->validatePass(password);
        if (!validate)
        {
            std::cout << "Неверный пароль: " << std::endl;
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

void IChatInterface::printMessages(std::vector<std::shared_ptr<Message>> &messages, const std::string &&emptyMsg, const std::string &&beforeMsg, const uint &start) const
{
    if (messages.empty())
    {
        std::cout << emptyMsg << std::endl;
        return;
    }
    uint k = start;
    for (auto &i : messages)
    {
        std::cout << beforeMsg << "[" << ++k << "]" << std::endl;
        auto user = usersDB.getUserByID(i->getAuthorID());
        user->printData();
        i->printData();
    }
}

chat::Results IChatInterface::listNavigation(uint &start, uint &perPage, uint &end, chat::Pagination &mode, uint max)
{
    UserInput<std::string, chat::Pagination> selectOption(std::string(),
                                                          "Выберите опцию:"
                                                          "\nэнс - элементов на странице;"
                                                          "\nпкэ - перейти к элементу №...;"
                                                          "\nсбр - сброс настроек (всегда последние 10 элементов);"
                                                          "\nн - вернуться в чат;"
                                                          "\nВведите значение: ",
                                                          "Неверный ввод.", 4);
    selectOption.addInputs("энс", "пкэ", "сбр", "н");
    selectOption.addOutputs(chat::Pagination::msg_per_page, chat::Pagination::select_message, chat::Pagination::select_last_page, chat::Pagination::in_chat);

    UserInput<int, int> getInt(std::string(), std::string(), "Неверный ввод");

    switch (selectOption.IOgetline())
    {
    case chat::Pagination::msg_per_page:
        getInt.setMainMessage("Укажите количество элементов на странице: ");
        perPage = getInt.IOcinThrough();
        if (perPage < 1)
        {
            perPage = 1;
        }
        break;
    case chat::Pagination::select_message:
        getInt.setMainMessage("Укажите номер элемента (1 - " + std::to_string(max) + "): ");
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
