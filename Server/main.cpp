#include <memory>
#include "../Misc/Misc.h"

#if defined(_WIN64) || defined(_WIN32)
#include "WinServer/WinServer.h"
#elif defined(__linux__)
#include "LinuxServer\LinuxServer.h"
#endif

typedef unsigned int uint;

const int PORT{7777}; // Порт сервера

int main(int argc, const char *argv[])
{
    std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
    server_socket();
    return 0;
}