#include <memory>
#include "../Misc/Misc.h"

#if defined(_WIN64) || defined(_WIN32)

#include "WinServer/WinServer.h"

#elif defined(__linux__)

#include "LinuxServer/LinuxServer.h"

#endif

typedef unsigned int uint;

int main(int argc, const char *argv[])
{
    /**
     * Порт можно указать в параметрах запуска
     * Если ничего не указано, порт = 7777
     */
    std::string port_s;
    if (argc >= 2)
    {
        port_s = argv[1];
    }
    else
    {
        port_s = "7777";
    }

    const auto PORT = port_s.data();
    std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    server_socket(PORT);

    return 0;
}