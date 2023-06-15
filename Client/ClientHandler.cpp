#include "ClientHandler.h"

void ClientHandler::Run()
{
    const uint buffer_len = in_buffer.size();
    char in_buf[buffer_len];
    Misc::writeVectorBuffer(in_buf, in_buffer);
    /*
    Входящий пакет должен иметь формат
    |I - Запрос числа|clear 0-1|PAGE_NAME 8b|data_size|data|
                                     1         2           10
    */

    std::string msg = Misc::getString(in_buf, 10);
    if (in_buf[1] == 1)
        system(clear);
    Misc::printMessage(msg);

    if (in_buf[0] == 'I')
    {
        UserInput<uint, uint> ui;
        ui.getThroughIO();
    }
    else
    {
    }
}
