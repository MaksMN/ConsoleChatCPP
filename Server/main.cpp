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

    DBClient d;
    d.initialise();
    d.DBprovider()->initialize();
    uint db_errno = 0;

    auto mmm = d.DBprovider()->getCount("users", "1", db_errno);

    auto uuu = d.DBprovider()->getUserByID(1, db_errno);
    int a = 0;

#if defined(_WIN64) || defined(_WIN32)
    system("pause");
#endif

    return 0;
}