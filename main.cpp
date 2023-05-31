#include "Chat/ChatRun.h"
#include <io.h>
#include <fcntl.h>
typedef unsigned int uint;

// команда ОС для очистки консоли
const char clear[]{"cls"};

/**
 * Убрать предустановленных тестовых пользователей и сообщения можно в Chat/ChatRun.cpp
 * Директива #define PRESETS
 * Все предустановленные пользователи имеют пароль pass, их логины можно увидеть в чате
 */

int main(int argc, const char *argv[])
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
    ChatRun chat;
    chat.Run();
}