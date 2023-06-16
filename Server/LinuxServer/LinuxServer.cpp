#include "LinuxServer.h"
#if defined(__linux__)
// Номер порта, который будем использовать для приема и передачи

char data_buffer[DATA_BUFFER];
char cmd_buffer[CMD_BUFFER];

int socket_file_descriptor, message_size;
socklen_t length;
const char *end_string = "end";
struct sockaddr_in serveraddress, client;
void server()
{
    ServerHandler handler(data_buffer, cmd_buffer);
    handler.InitialiseDB();

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
        message_size = recvfrom(socket_file_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, (struct sockaddr *)&client, &length);

        /*
        // отключение клиента
        buffer[message_size] = '\0';
        if (strcmp(buffer, end_string) == 0)
        {
            std::cout << "Server is Quitting" << std::endl;
            close(socket_file_descriptor);
            exit(0);
        }

        */

        handler.Run(); // обработка входящих данных и формирование ответа

        // Отправка пакета команд
        sendto(socket_file_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, (struct sockaddr *)&client, sizeof(client));

        // Отправка пакета с данными
        sendto(socket_file_descriptor, data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client, sizeof(client));
    }

    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
}

/**
 * Принцип такой:
 * 1. От клиента приходит сообщение в котором первые 4 байта это имя команды, дальше данные команды.
 * 2. Отправляем данные в обработчик команд.
 * 3. Обработчик команд возвращает сообщение и его размер.
 * 4. Отправляем клиенту 8 байт. Первый блок 4 байта - метка TAKE. Второй блок - длина данных следующего сообщения.
 * 5. Следующим сообщением отправляем данные клиенту.
 */

#endif