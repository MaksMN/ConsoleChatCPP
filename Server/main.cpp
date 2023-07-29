#include <memory>
#include "../Misc/Misc.h"
#include "DBClient/DBClient.h"

#if defined(_WIN64) || defined(_WIN32)

// #include "WinServer/WinServer.h"
#include <io.h>
#include <fcntl.h>

#elif defined(__linux__)

#include "LinuxServer/LinuxServer.h"

#endif

typedef unsigned int uint;

const char config_file[]{".console_chat/server.ini"};

int main(int argc, const char *argv[])
{
#if defined(_WIN64) || defined(_WIN32)
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#endif

    auto t = Misc::getConfigValue(config_file, "DB", "dbpass");
    DBClient d;
    d.initialise();
    d.DBprovider()->initialize();
    uint db_errno = 0;
    bool login_busy = false;
    bool email_busy = false;
    auto u = d.DBprovider()->getUserByLogin("admin", db_errno);

    auto u2 = d.DBprovider()->getUserByID(55, db_errno);

    auto us = std::make_shared<User>("xxxxxxx", "emaiiila", "dfcza", "asdfa", "pass");
    auto tt = d.DBprovider()->addUser(us, login_busy, email_busy, db_errno);

    int a = 0;

#if defined(_WIN64) || defined(_WIN32)
    system("pause");
#endif

    return 0;
}