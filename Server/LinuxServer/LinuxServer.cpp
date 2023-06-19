#include "LinuxServer.h"
#if defined(__linux__)
// Номер порта, который будем использовать для приема и передачи

char data_buffer[DATA_BUFFER];
char cmd_buffer[CMD_BUFFER];

int socket_file_descriptor, message_size;
socklen_t length;

struct sockaddr_in serveraddress, client;
int server_socket(char port[])
{
    ServerHandler handler(data_buffer, cmd_buffer);
    handler.InitialiseDB();

    // Создадим UDP сокет
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим порт для соединения
    serveraddress.sin_port = htons(atoi(port));
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    while (handler.getWork())
    {
        length = sizeof(client);

        int iResult = recvfrom(socket_file_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, (struct sockaddr *)&client, &length);

        handler.Run(); // обработка входящих данных и формирование ответа

        // Отправка пакета команд
        sendto(socket_file_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, (struct sockaddr *)&client, sizeof(client));

        // Отправка пакета с данными
        sendto(socket_file_descriptor, data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client, sizeof(client));
    }

    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
    return 0;
}

#endif