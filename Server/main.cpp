#include "Socket/Server.h"
#include <memory>
#include "../Misc/Misc.h"

typedef unsigned int uint;

const int PORT{7777}; // Порт сервера

int main(int argc, const char *argv[])
{
    std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
    processRequest();
    return 0;
}