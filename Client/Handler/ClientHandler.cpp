#include "ClientHandler.h"

ClientHandler::ClientHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]) : data_buffer(_data_buffer), cmd_buffer(_cmd_buffer) {}

void ClientHandler::Initialise()
{
    Misc::writeStringBuffer("00", cmd_buffer, 0, false);
    Misc::writeStringBuffer("MAIN_PAGE", cmd_buffer, 2);
    std::string data =
        "Вы находитесь на главной странице чата. Введите команду chat чтобы начать общение.\n"
        "Команда HELLO - пинг сервера.\n"
        "Введите команду: ";
    Misc::writeStringBuffer(data, data_buffer);
}

void ClientHandler::Run()
{
    /*

    Исходящий пакет
    |session_key|login_size|login|page_size|    PAGE_TEXT    |size| CMD_TEXT        |
                8         12     |12+login_size              |pagePos + 4 + pageSize
    Если ожидается ввод числа, записываем его в блок size CMD_TEXT

    Входящий пакет команд
    |I - Запрос числа|clear 0-1|size|PAGE_TEXT|
                               2

    */

    // получение команды
    if (cmd_buffer[1] == 1)
        system(clear);

    pageText = Misc::getString(cmd_buffer, 2);
    dataText = Misc::getString(data_buffer);
    Misc::printMessage(dataText, false);

    // формируем ответ
    pagePos = 12 + login_size;
    cmdPos = pagePos + 4 + pageText.size();

    Misc::writeUlongBuffer(session_key, cmd_buffer);
    Misc::writeStringBuffer(login, cmd_buffer, 8);
    Misc::writeStringBuffer(pageText, cmd_buffer, pagePos);

    if (cmd_buffer[0] == 'I')
    {
        uint n = userInputInt.getThroughIO();
        Misc::writeIntBuffer(n, cmd_buffer, cmdPos);
    }
    else
    {
        std::string s = userInputStr.getStringIO();
        Misc::writeStringBuffer(s, cmd_buffer, cmdPos);
    }

    return;
}
