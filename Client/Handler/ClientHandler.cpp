#include "ClientHandler.h"

ClientHandler::ClientHandler(char (&_cmd_buffer)[CMD_BUFFER]) : cmd_buffer(_cmd_buffer) {}

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
    buffer.setSessionKey(session_key);
    buffer.setPaginationMode(sv::last_page);
    buffer.setPgPerPage(10);
    buffer.setPgStart(0);
    buffer.setPgEnd(0);
    buffer.clearPmUserID();

    // Динамические данные
    buffer.writeDynData(login, "MAIN_PAGE", "/chat");

    data_text =
        "Вы запустили клиент чата.\n"
        "Введите команду /chat чтобы начать общение.\n"
        "Команда /help - справка.\n"
        "Команда /hello - опрос сервера.\n"
        "Введите команду: ";
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

    // if (Misc::getInt(data_buffer) > DATA_BUFFER || Misc::getString(cmd_buffer, 11, 0) == "BAD_REQUEST")
    // {
    //     Misc::printMessage("Сообщение от клиента: ", true);
    //     Misc::printMessage("Что-то пошло не так. От сервера пришли данные неверной длинны.", true);
    //     Misc::printMessage("Сессия будет сброшена.", true);
    //     Misc::printMessage("В будущем мы попробуем научить чат более корректно реагировать на сбои сервера.", true);
    //     Misc::printMessage("Введите команду /chat, чтобы продолжить: ", true);

    //     // Статические данные
    //     buffer.setSessionKey(Misc::getRandomKey());
    //     buffer.createFlags(sv::get_string);
    //     cmd_buffer[DYN_DATA_PTR_ADDR] = DYN_DATA_ADDR;
    //     buffer.setPaginationMode(sv::last_page);
    //     buffer.setPgPerPage(10);
    //     buffer.setPgStart(1);
    //     buffer.setPgEnd(0);
    //     // Динамические данные
    //     buffer.writeDynData("Guest", "MAIN_PAGE", "NONE");
    // }

    Misc::printMessage(data_text, false);

    // запишем в буфер текст который отобразится если сервер отвалится
    data_text = "Сервер не ответил на ваш запрос.\nВведите команду: ";

    // пишем ответ серверу
    if (buffer.hasFlag(sv::no_input))
    {
        buffer.createFlags(sv::get_string);
        return;
    }
    if (buffer.hasFlag(sv::get_int))
    {
        uint n = userInputInt.getThroughIO();
        buffer.writeDynDataPos(n, CMD_TEXT_COUNT);
    }
    else
    {
        std::string s = userInputStr.getStringIO();
        if (s.size() == 0)
        {
            buffer.writeDynDataPos("/update", CMD_TEXT_COUNT);
        }
        else
        {
            buffer.writeDynDataPos(s, CMD_TEXT_COUNT);
        }

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

void ClientHandler::setDataText(const std::string &text)
{
    data_text = text;
}

void ClientHandler::setDataText(const std::string &&text)
{
    setDataText(text);
}
