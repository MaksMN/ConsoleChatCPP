#include "ChatRun.h"
#include <fstream>

// #define PRESETS_MSG
// #define PRESETS_USERS

void ChatRun::Run()
{
    system(clear);
    std::cout << "Логин сервисного администратора: admin, пароль: 1234" << std::endl;
    std::cout << "Рекомендуем сменить пароль. Войдите в чат -> настройки профиля." << std::endl;
    std::cout << "Это сообщение больше не будет отображаться." << std::endl;
    std::cout << std::endl;
    bool showStartMessage = true;
#ifdef PRESETS_USERS
    std::filesystem::remove("users");
    User admin(0, "admin", "Администратор", "1234");
    admin.toAdmin();
    admin.writeData();
    // Предустановленные пользователи
    User u1(1, "vasya", "Василий", "pass");
    u1.writeData();
    User u2(2, "maria", "Маша", "pass");
    u2.writeData();
    User u3(3, "vano", "Иван", "pass");
    u3.writeData();
    User u4(4, "igor", "Игорь", "pass");
    u4.writeData();
#endif

    usersDB.updateFromFile();

#ifdef PRESETS_MSG
    pubMessagesDB.addMessage(0, 0, "Всем привет!!!", msg::public_);
    pubMessagesDB.addMessage(1, 1, "И тебе привет", msg::public_);
    pubMessagesDB.addMessage(2, 2, "Всем чмоки в этом чате :-)", msg::public_);
    pubMessagesDB.addMessage(3, 3, "Какие у нас планы?", msg::public_);
    pubMessagesDB.addMessage(4, 4, "Предлагаю всем завтра рвануть на шашлыки", msg::public_);
#endif

    UserInput<std::string, chat::Results> coreAreaPage("Главная станица",
                                                       "Выберите действия: ч - Чат, а - Раздел администратора, в - Выход из программы ",
                                                       "Неверный ввод",
                                                       3 // количество возможных вариантов ввода
    );

    // создание возможных вариантов ввода
    coreAreaPage.addInputs("ч", "а", "в");
    // Соответствующие варианты вывода
    coreAreaPage.addOutputs(chat::in_chat_area, chat::in_admin_area, chat::exit_);
    chat::Results userInputResult = chat::Results::exit_;
    do
    {
        if (!showStartMessage)
            system(clear);
        showStartMessage = false;

        if (userInputResult == chat::admin_login_fail)
            std::cout << "Вы не смогли авторизоваться в разделе администратора." << std::endl;

        if (userInputResult == chat::back_)
            std::cout << "Вы вернулись на главную страницу чата." << std::endl;

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
    std::cout << "Вы вышли из чата. До свидания." << std::endl;
    return;
}