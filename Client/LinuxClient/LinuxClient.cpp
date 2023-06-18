#include "LinuxClient.h"
#if defined(__linux__)
char data_buffer[DATA_BUFFER];
char cmd_buffer[CMD_BUFFER];
int socket_descriptor;
struct sockaddr_in serveraddress;

int client_socket(char server_address[], char port[])
{
    ClientHandler handler(data_buffer, cmd_buffer);
    handler.Initialise();

    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr(server_address);
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(atoi(port));
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Создадим сокет
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    // Установим соединение с сервером
    if (connect(socket_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress)) < 0)
    {
        std::cout << std::endl
                  << " Something went wrong Connection Failed" << std::endl;
        exit(1);
    }
    struct timeval tv;
    tv.tv_sec = 30;
    tv.tv_usec = 0;
    auto in = setsockopt(socket_descriptor, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof tv);
    while (handler.getWork())
    {
        handler.Run();
        if (!handler.getWork())
            break;

        // отправка команды на сервер
        sendto(socket_descriptor, cmd_buffer, CMD_BUFFER, 0, nullptr, sizeof(serveraddress));

        // получение команды от сервера
        recvfrom(socket_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, nullptr, nullptr);

        // получение данных с сервера
        recvfrom(socket_descriptor, data_buffer, sizeof(data_buffer), 0, nullptr, nullptr);
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
    return 0;
}
#endif