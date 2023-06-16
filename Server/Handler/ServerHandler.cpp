#include "ServerHandler.h"

ServerHandler::ServerHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]) : data_buffer(_data_buffer), cmd_buffer(_cmd_buffer) {}

void ServerHandler::InitialiseDB()
{
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
}

void ServerHandler::Run()
{
    /*
    Входящий пакет
    |session_key|login_size|login|page_size|    PAGE_TEXT    |size| CMD_TEXT        |
                8         12     |12+login_size              |pagePos + 4 + pageSize

    Исходящий пакет команд
    |I - Запрос числа|clear 0-1|size|PAGE_TEXT|
                               2
    */
    session_key = Misc::getLong(cmd_buffer);
    login_size = Misc::getInt(cmd_buffer, 8);

    if (12 + login_size >= CMD_BUFFER || pageSize + 4 > CMD_BUFFER)
    {
        badRequest();
        return;
    }

    pagePos = 12 + login_size;
    pageSize = Misc::getInt(cmd_buffer, pagePos);
    if (pageSize + 4 >= CMD_BUFFER)
    {
        badRequest();
        return;
    }

    cmdPos = pagePos + 4 + pageSize;
    cmdSize = Misc::getInt(cmd_buffer, cmdPos);
    if (cmdSize + 4 > CMD_BUFFER)
    {
        badRequest();
        return;
    }

    login = Misc::getString(cmd_buffer, 8);
    pageText = Misc::getString(cmd_buffer, pagePos);
    cmdText = Misc::getString(cmd_buffer, cmdPos);

    if (cmdText.compare("HELLO") == 0)
    {
        Misc::writeStringBuffer("00", cmd_buffer, 0, false);
        Misc::writeStringBuffer(pageText, cmd_buffer, 2);
        Misc::writeStringBuffer("Привет, " + login + "! Я сервер.\nВведите команду: ", data_buffer);

        return;
    }

    return;
}

void ServerHandler::badRequest()
{
    Misc::writeStringBuffer("01", cmd_buffer, 0, false);
    Misc::writeStringBuffer("WRONG", cmd_buffer, 2);
    Misc::writeStringBuffer("Wrong command", data_buffer);
}
