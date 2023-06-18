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
    buffer.createFlags(sv::get_string);
    cmd_buffer[DYN_DATA_PTR_ADDR] = DYN_DATA_ADDR;
    buffer.setSessionKey(session_key);

    // Динамические данные
    buffer.writeDynData(login, "MAIN_PAGE", "NONE");

    std::string data =
        "Вы запустили клиент чата.\n"
        "Введите команду /chat чтобы начать общение.\n"
        "Команда /help - справка.\n"
        "Команда /hello - опрос сервера.\n"
        "Введите команду: ";
    Misc::writeStringBuffer(data, data_buffer);
}

void ClientHandler::Run()
{
    if (buffer.hasFlag(sv::clear_console))
        system(clear);
    buffer.removeFlag(sv::clear_console);

    if (buffer.hasFlag(sv::write_session))
    {
        session_key = buffer.getSessionKey();
    }

    if (Misc::getInt(data_buffer) > DATA_BUFFER)
    {
        Misc::printMessage("Что-то пошло не так. От сервера пришли данные неверной длинны.", false);
    }
    else
    {
        Misc::printMessage(Misc::getString(data_buffer), false);
    }

    // запишем в буфер текст который отобразится если сервер отвалится
    Misc::writeStringBuffer("Сервер не ответил на ваш запрос.\nВведите команду: ", data_buffer);

    if (buffer.hasFlag(sv::get_int))
    {
        uint n = userInputInt.getThroughIO();
        buffer.writeDynDataPos(n, CMD_TEXT_COUNT);
    }
    else
    {
        std::string s = userInputStr.getStringIO();
        buffer.writeDynDataPos(s, CMD_TEXT_COUNT);

        if (s == "/quit")
        {
            quit();
        }
    }

    return;
}

bool ClientHandler::getWork()
{
    return work;
}

void ClientHandler::quit()
{
    work = false;
}
