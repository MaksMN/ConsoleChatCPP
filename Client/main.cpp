#include <iostream>
#include "../Misc/Misc.h"
#if defined(_WIN64) || defined(_WIN32)

#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

#include "WinClient\WinClient.h"
// команда ОС для очистки консоли
const char clear[]{"cls"};
#elif defined(__linux__)

#include "LinuxClient/LinuxClient.h"
// команда ОС для очистки консоли
const char clear[]{"clear"};

#endif

int main(int argc, char **argv)
{
#if defined(_WIN64) || defined(_WIN32)

    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#endif

    std::string port_s;
    std::string server_address_s = "127.0.0.1";
    if (argc == 2)
    {
        port_s = argv[1];
    }
    else
    {
        port_s = "7777";
    }

    if (argc == 3)
    {
        server_address_s = argv[1];
        port_s = argv[2];
    }
    else
    {
        port_s = "7777";
    }

    const auto port = port_s.data();
    const auto server_address = server_address_s.data();
    std::cout << "CLIENT IS ESTABLISHING A CONNECTION WITH SERVER THROUGH PORT: " << port << " WITHIN A LOCAL SYSTEM" << std::endl;
    client_socket(server_address, port);

    system("pause");
    return 0;
}