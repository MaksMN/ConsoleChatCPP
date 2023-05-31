#include "Chat/ChatRun.h"
typedef unsigned int uint;

// команда ОС для очистки консоли
const char clear[]{"clear"};

/**
 * Убрать предустановленных тестовых пользователей и сообщения можно в Chat/ChatRun.cpp
 * Директива #define PRESETS
 * Все предустановленные пользователи имеют пароль pass, их логины можно увидеть в чате
 */

int main(int argc, const char *argv[])
{
    ChatRun chat;
    chat.Run();
}