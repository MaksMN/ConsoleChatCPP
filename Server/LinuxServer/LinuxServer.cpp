#include "LinuxServer.h"
#if defined(__linux__)
// Номер порта, который будем использовать для приема и передачи

char cmd_buffer[CMD_BUFFER];
char data_buffer[DATA_BUFFER];
int socket_file_descriptor, message_size;
socklen_t length;

struct sockaddr_in serveraddress, client;
int server_socket(char port[])
{
    ServerHandler handler(cmd_buffer);
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

        std::string data_text = Misc::ltrimString(handler.getDataText(), (DATA_BUFFER - 4) * 254);

        if (data_text.size() == 0)
            data_text = " ";
        unsigned char data_parts = (data_text.size() / (DATA_BUFFER - 4)) + 1;

        cmd_buffer[1] = data_parts;
        // Отправка пакета команд
        sendto(socket_file_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, (struct sockaddr *)&client, sizeof(client));

        for (int i = 0; i < data_text.size(); i += DATA_BUFFER - 4)
        {
            Misc::writeStringBuffer(data_text.substr(i, DATA_BUFFER - 4), data_buffer);
            sendto(socket_file_descriptor, data_buffer, sizeof(data_buffer), 0, (struct sockaddr *)&client, sizeof(client));
        }
    }

    // закрываем сокет, завершаем соединение
    close(socket_file_descriptor);
    return 0;
}

#endif