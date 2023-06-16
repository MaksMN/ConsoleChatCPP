#include <iostream>
#if defined(_WIN64) || defined(_WIN32)

#include "WinClient\WinClient.h"
// команда ОС для очистки консоли
const char clear[]{"cls"};

#elif defined(__linux__)

#include "LinuxClient/LinuxClient.h"
// команда ОС для очистки консоли
const char clear[]{"clear"};

#endif

const int PORT{7777}; // Порт сервера
int main(int argc, char **argv)
{
    std::cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << std::endl;
    client(argc, argv);
    return 0;
}