#pragma once
#include <iostream>
#include "IChatInterface.h"
#include "ChatUserInterface.h"
#include "ChatAdminInterface.h"
#include "UserInput.h"

class ChatRun
{
private:
public:
    ChatRun();
    ~ChatRun() = default;
};

ChatRun::ChatRun()
{
    std::unique_ptr<DB> db = std::make_unique<DB>();
    User service_admin(L"admin", L"admin", L"1234");
    service_admin.setIsAdmin(true);
    db->addUser(service_admin);                                                         // Service Admin userID = 1;
    db->addUser(L"complaint_bot", L"complaint_bot", L"uijyh6758r9ifSDFGH&^tf34rdfcvgtre"); // бот которому отсылаются жалобы пользователей. userID = 2;

    std::wcout << L"Логин сервисного администратора: admin, пароль: 1234" << std::endl;
    std::wcout << L"Рекомендуем сменить пароль. Войдите в чат -> настройки профиля." << std::endl;
    std::wcout << L"Это сообщение больше не будет отображаться." << std::endl;
    std::wcout << std::endl;

    ChatUserInterface userInterface;
    ChatAdminInterface adminInterface;

    chat::Results userInput;
    chat::Results result;
    // Объект страницы
    UserInput<std::wstring, chat::Results> coreAreaPage(L"Главная станица",
                                                       L"Выберите действия: ч - Чат, а - Раздел администратора, в - Выход из программы ",
                                                       L"Неверный ввод",
                                                       3 // количество возможных вариантов ввода
    );

    // создание возможных вариантов ввода
    coreAreaPage.addInputs(L"ч", L"а", L"в");
    // Соответствующие варианты вывода
    coreAreaPage.addOutputs(chat::chat, chat::admin, chat::app_exit);

    do
    {
        userInput = coreAreaPage.IOgetline();
        switch (userInput)
        {
        case chat::chat:
            system(clear);
            result = userInterface.run(std::move(db));
            db = std::move(userInterface.db);
            break;
        case chat::admin:
            result = adminInterface.run(std::move(db));
            db = std::move(adminInterface.db);
            break;
        case chat::app_exit:
            result = chat::app_exit;
            break;
        default:
            std::wcout << L"Неверный ввод" << std::endl;
            break;
        }
    } while (result != chat::app_exit);
    std::wcout << L"Вы вышли из чата. До свидания." << std::endl;
}