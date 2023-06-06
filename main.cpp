#include "Chat/ChatRun.h"
typedef unsigned int uint;

// команда ОС для очистки консоли
const char clear[]{"clear"};

/**
 * Предустановленные пользователи и сообщения находятся в папке PRESETS
 * Скопируйте их в рабочий каталог приложения.
 * Все предустановленные пользователи имеют пароль pass, их логины можно увидеть в чате
 * Пути к файлам базы данных можно задать в Chat/ChatRun.h
 */

int main(int argc, const char *argv[])
{
    ChatRun chat;
    chat.Run();
}