#include "Handler.h"

Handler::Handler(char (&buffer)[BUFFER]) : _buffer(buffer) {}

void Handler::InitialiseDB()
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

void Handler::Run()
{
    /*
    структура входящего пакета
    |session_key|login_size|login|  CMD 8bytes |
                8         12     |12+login_size|
    */

    session_key = Misc::getLong(_buffer);
    login_size = Misc::getInt(_buffer, 8);

    if (12 + login_size >= BUFFER)
    {
        message.clear();
        message = Misc::writeVectorBuffer("WRONGPAC");
        return;
    }

    cmdPos = 12 + login_size;
    cmdText = Misc::getString(_buffer, cmdPos);

    if (cmdText == "HELLO___")
    {
        char msg[38]{};
        Misc::writeStringBuffer("ECHO____", msg, 0, false);
        Misc::writeStringBuffer("Hello, client! I am server.", msg, 8);
        message = Misc::writeVectorBuffer(msg, 38);
        return;
    }
}
