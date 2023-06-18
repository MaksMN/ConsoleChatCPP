#include "ClientHandler.h"

ClientHandler::ClientHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]) : data_buffer(_data_buffer), cmd_buffer(_cmd_buffer) {}

void ClientHandler::Initialise()
{
    // При инициализации клиента надо сбросить и сформировать новый сессионный ключ,
    // который будет действовать во время всего сеанса.
    // он не должен быть равным нулю.
    session_key = 0;
    while (session_key == 0)
    {
        session_key = Misc::getRandomKey();
    }

    // Статические данные
    Misc::writeStringBuffer("00", cmd_buffer, 0, false);
    Misc::writeUlongBuffer(session_key, cmd_buffer, 2);

    // Динамические данные
    Misc::writeStringBuffer(login, cmd_buffer, 10);
    Misc::writeStringBuffer("MAIN_PAGE", cmd_buffer, Misc::findDynamicData(cmd_buffer, 10, 1));
    Misc::writeStringBuffer("NONE", cmd_buffer, Misc::findDynamicData(cmd_buffer, 10, 2));

    std::string data =
        "Вы запустили клиент чата.\n"
        "Введите команду chat чтобы начать общение.\n"
        "Команда /help - справка.\n"
        "Команда /hello - опрос сервера.\n"
        "Введите команду: ";
    Misc::writeStringBuffer(data, data_buffer);
}

void ClientHandler::Run()
{
    /*

    Командный пакет
    |Запрос у клиента I/S (1)|clear 0-1 (1)|session_key (8)|login_size(4)|login|page_size(4)|PAGE_TEXT|cmd_size(4)|CMD_TEXT|
    0                        1             2               10                  1                      2
    |  static                                              | dynamic
    Запрос у клиента: I - число S - строка
    Если ожидается ввод числа, записываем его в блок cmd_size

    */

    if (cmd_buffer[1] == 1)
        system(clear);

    if (Misc::getInt(data_buffer) > DATA_BUFFER)
    {
        Misc::printMessage("Что-то пошло не так. От сервера пришли данные неверной длинны.", false);
    }
    else
    {
        Misc::printMessage(Misc::getString(data_buffer), false);
        clearConsole(false);
    }

    // запишем в буфер текст который отобразится если сервер отвалится
    Misc::writeStringBuffer("Сервер не ответил на ваш запрос.\nВведите команду: ", data_buffer);

    uint cmd_pos = Misc::findDynamicData(cmd_buffer, 10, 2);

    if (cmd_buffer[0] == 'I')
    {
        uint n = userInputInt.getThroughIO();
        Misc::writeIntBuffer(n, cmd_buffer, cmd_pos);
    }
    else
    {
        std::string s = userInputStr.getStringIO();
        Misc::writeStringBuffer(s, cmd_buffer, cmd_pos);

        if (s == "/quit")
        {
            quit();
        }
    }

    Misc::writeUlongBuffer(session_key, cmd_buffer);

    return;
}

void ClientHandler::clearConsole(bool status)
{
    cmd_buffer[1] = status;
}

void ClientHandler::inputClient(char input)
{
    cmd_buffer[0] = input;
}

bool ClientHandler::getWork()
{
    return work;
}

void ClientHandler::quit()
{
    work = false;
}
