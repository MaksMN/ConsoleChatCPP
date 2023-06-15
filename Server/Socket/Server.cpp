#include "Server.h"
// Номер порта, который будем использовать для приема и передачи

char buffer[BUFFER];
char take[8]; // |TAKE|size| Высылается клиенту перед основным сообщением, передает размер сообщения
int socket_file_descriptor, message_size;
socklen_t length;
const char *end_string = "end";
struct sockaddr_in serveraddress, client;
void processRequest()
{
    Handler handler(buffer);
    handler.InitialiseDB();
    Misc::writeStringBuffer("TAKE", take);
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
        // buffer это сообщение от клиента

        // message это сообщение отправляемое клиенту

        handler.Run(); // обработка входящих данных и формирование ответа

        const uint msg_size = handler.message.size();      // размер ответа
        char message[msg_size];                            // буфер для ответа
        Misc::writeVectorBuffer(message, handler.message); // записываем ответ в буфер

        // Сначала отправляем клиенту take - пакет с размером сообщения
        Misc::writeIntBuffer(msg_size, take, 4);
        sendto(socket_file_descriptor, take, 8, 0, (struct sockaddr *)&client, sizeof(client));

        // Отправка основного сообщения
        sendto(socket_file_descriptor, message, msg_size, 0, (struct sockaddr *)&client, sizeof(client));
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