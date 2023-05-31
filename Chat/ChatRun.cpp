#include "ChatRun.h"
#define PRESETS

void ChatRun::Run()
{
    system(clear);
    std::wcout << L"Логин сервисного администратора: admin, пароль: 1234" << std::endl;
    std::wcout << L"Рекомендуем сменить пароль. Войдите в чат -> настройки профиля." << std::endl;
    std::wcout << L"Это сообщение больше не будет отображаться." << std::endl;
    std::wcout << std::endl;
    bool showStartMessage = true;

    auto admin = usersDB.addUser(L"admin", L"Администратор", L"1234");
    admin->toAdmin();

#ifdef PRESETS
    // Предустановленные пользователи
    usersDB.addUser(L"vasya", L"Василий", L"pass");
    usersDB.addUser(L"maria", L"Маша", L"pass");
    usersDB.addUser(L"vano", L"Иван", L"pass");
    usersDB.addUser(L"igor", L"Игорь", L"pass");

    pubMessagesDB.addMessage(0, 0, L"Всем привет!!!", msg::public_);
    pubMessagesDB.addMessage(1, 1, L"И тебе привет", msg::public_);
    pubMessagesDB.addMessage(2, 2, L"Всем чмоки в этом чате :-)", msg::public_);
    pubMessagesDB.addMessage(3, 3, L"Какие у нас планы?", msg::public_);
    pubMessagesDB.addMessage(4, 4, L"Предлагаю всем завтра рвануть на шашлыки", msg::public_);
#endif

    UserInput<std::wstring, chat::Results> coreAreaPage(L"Главная станица",
                                                       L"Выберите действия: ч - Чат, а - Раздел администратора, в - Выход из программы ",
                                                       L"Неверный ввод",
                                                       3 // количество возможных вариантов ввода
    );

    // создание возможных вариантов ввода
    coreAreaPage.addInputs(L"ч", L"а", L"в");
    // Соответствующие варианты вывода
    coreAreaPage.addOutputs(chat::in_chat_area, chat::in_admin_area, chat::exit_);
    chat::Results userInputResult = chat::Results::exit_;
    do
    {
        if (!showStartMessage)
            system(clear);
        showStartMessage = false;

        if (userInputResult == chat::admin_login_fail)
            std::wcout << L"Вы не смогли авторизоваться в разделе администратора." << std::endl;

        if (userInputResult == chat::back_)
            std::wcout << L"Вы вернулись на главную страницу чата." << std::endl;

        auto userInput = coreAreaPage.IOgetline();
        switch (userInput)
        {
        case chat::Results::in_chat_area:
            system(clear);
            userInputResult = userInterface.run();
            break;
        case chat::Results::in_admin_area:
            system(clear);
            userInputResult = adminInterface.run();
            break;
        case chat::exit_:
            userInputResult = chat::exit_;
        default:
            break;
        }
    } while (userInputResult != chat::Results::exit_);
    system(clear);
    std::wcout << L"Вы вышли из чата. До свидания." << std::endl;
    return;
}