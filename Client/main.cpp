#include "Client.h"
#if defined(_WIN64) || defined(_WIN32)
// команда ОС для очистки консоли
const char clear[]{"cls"};
#else
// команда ОС для очистки консоли
const char clear[]{"clear"};
#endif

const int PORT{7777}; // Порт сервера
int main()
{
    std::cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << PORT << std::endl;
    sendRequest();
    return 0;
}