#include "ChatRun.h"
#include <fstream>

void ChatRun::Run()
{
    system(clear);
    std::cout << "Логин сервисного администратора: admin, пароль: 1234" << std::endl;
    std::cout << "Рекомендуем сменить пароль. Войдите в чат -> настройки профиля." << std::endl;
    std::cout << "Это сообщение больше не будет отображаться." << std::endl;
    std::cout << std::endl;
    bool showStartMessage = true;

    // Сервисный админ должен быь всегда
    if (!std::filesystem::exists(usersDB.getDBfilePath()))
    {
        User admin(0, "admin", "Администратор", "1234", usersDB.getDBfilePath());
        admin.toAdmin();
        admin.writeData();
    }

    usersDB.updateFromFile();
    pubMessagesDB.updateFromFile();
    privMessagesDB.updateFromFile();

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