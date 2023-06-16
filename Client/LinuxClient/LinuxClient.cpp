#include "LinuxClient.h"
#if defined(__linux__)
char data_buffer[DATA_BUFFER];
char cmd_buffer[CMD_BUFFER];
int socket_descriptor;
struct sockaddr_in serveraddress;

void sendRequest()
{
    ClientHandler handler(data_buffer, cmd_buffer);
    handler.Initialise();

    // Укажем адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    // Зададим номер порта для соединения с сервером
    serveraddress.sin_port = htons(PORT);
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

    while (1)
    {
        handler.Run();
        /*
        if (strcmp(message, "end") == 0)
        {
            sendto(socket_descriptor, message, BUFFER, 0, nullptr, sizeof(serveraddress));
            std::cout << "Client work is done.!" << std::endl;
            close(socket_descriptor);
            exit(0);
        }
        else
        {
            sendto(socket_descriptor, message, BUFFER, 0, nullptr, sizeof(serveraddress));
            std::cout << "Сообщение успешно отправлено на сервер: " << message << std::endl;
            std::cout << "Ожидание ответа от сервера..." << std::endl;
        }
        */

        sendto(socket_descriptor, cmd_buffer, CMD_BUFFER, 0, nullptr, sizeof(serveraddress));

        // получение команды от сервера
        recvfrom(socket_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, nullptr, nullptr);

        recvfrom(socket_descriptor, data_buffer, sizeof(data_buffer), 0, nullptr, nullptr);
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
}
#endif