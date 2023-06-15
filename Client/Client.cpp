#include "Client.h"

char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_descriptor;
struct sockaddr_in serveraddress;

void sendRequest()
{
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

        if (strcmp(message, "end") == 0)
        {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            std::cout << "Client work is done.!" << std::endl;
            close(socket_descriptor);
            exit(0);
        }
        else
        {
            sendto(socket_descriptor, message, MESSAGE_BUFFER, 0, nullptr, sizeof(serveraddress));
            std::cout << "Сообщение успешно отправлено на сервер: " << message << std::endl;
            std::cout << "Ожидание ответа от сервера..." << std::endl;
        }
        std::cout << "Сообщение получено с сервера: " << std::endl;
        recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, nullptr, nullptr);
        std::cout << buffer << std::endl;
    }
    // закрываем сокет, завершаем соединение
    close(socket_descriptor);
}