#include <memory>
#include "../Misc/Misc.h"

#if defined(_WIN64) || defined(_WIN32)

#include "WinServer/WinServer.h"
#include <io.h>
#include <fcntl.h>

#elif defined(__linux__)

#include "LinuxServer/LinuxServer.h"

#endif

typedef unsigned int uint;

int main(int argc, const char *argv[])
{
#if defined(_WIN64) || defined(_WIN32)
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#endif
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
#if defined(_WIN64) || defined(_WIN32)
    std::wcout << L"SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    std::wcout << L"Сервисный администратор: логин - admin пароль - 1234" << std::endl;
    std::wcout << L"Авторизуйтесь в клиенте и смените пароль командой /profile" << std::endl;
    std::wcout << L"Чтобы завершить работу сервера, авторизуйтесь в клиенте с администраторскими правами и введите команду /sv_quit" << std::endl;
#elif defined(__linux__)
    std::cout << "SERVER IS LISTENING THROUGH THE PORT: " << PORT << " WITHIN A LOCAL SYSTEM" << std::endl;
    std::cout << "Сервисный администратор: логин - admin пароль - 1234" << std::endl;
    std::cout << "Авторизуйтесь в клиенте и смените пароль командой /profile" << std::endl;
    std::cout << "Чтобы завершить работу сервера, авторизуйтесь в клиенте с администраторскими правами и введите команду /sv_quit" << std::endl;
#endif

    server_socket(PORT);

    return 0;
}