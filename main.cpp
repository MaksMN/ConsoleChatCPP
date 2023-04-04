#include "Project/ChatRun.h"
#include <io.h>
#include <fcntl.h>
// команда ОС для очистки консоли
const char clear[]{"cls"};

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    ChatRun run;
    return 0;
}