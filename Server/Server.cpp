#include "Server.h"
// Номер порта, который будем использовать для приема и передачи

char buffer[MESSAGE_BUFFER];
char message[MESSAGE_BUFFER];
int socket_file_descriptor, message_size;
socklen_t length;
const char *end_string = "end";
struct sockaddr_in serveraddress, client;
void processRequest()
{
    // Создадим UDP сокет
    socket_file_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    // Зададим порт для соединения
    serveraddress.sin_port = htons(PORT);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    // Привяжем сокет
    bind(socket_file_descriptor, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    while (1)
    {
        // Длина сообщения от клиента
        length = sizeof(client);
        message_size = recvfrom(socket_file_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &length);
        buffer[message_size] = '\0';
        if (strcmp(buffer, end_string) == 0)
        {
            std::cout << "Server is Quitting" << std::endl;
            close(socket_file_descriptor);
            exit(0);
        }
        std::cout << "Сообщение, полученное от клиента: " << buffer << std::endl;
        // ответим клиенту

        // std::string m("Hello ");
        // m += std::string(buffer);
        // for (int i{0}; i < m.length(); i++)
        // {
        //     message[i] = m[i];
        // }

        std::cout << "Enter reply message to the client: " << std::endl;
        std::cin >> message;

        sendto(socket_file_descriptor, message, MESSAGE_BUFFER, 0, (struct sockaddr *)&client, sizeof(client));
        std::cout << "Сообщение успешно отправлено клиенту: " << message << std::endl;
        std::cout << "Жду ответа от клиента..." << std::endl;
    }

    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}