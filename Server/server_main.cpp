#include "Server.h"

typedef unsigned int uint;

// команда ОС для очистки консоли
const char clear[]{"clear"};

int main(int argc, const char *argv[])
{
    std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    // Запускаем функцию обработки сообщений от клиентов и ответа на эти сообщения
    processRequest();
    return 0;
}