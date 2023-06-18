#include "WinClient.h"
#if defined(_WIN64) || defined(_WIN32)

int client_socket(char server_address[], char port[])
{
    char data_buffer[DATA_BUFFER];
    char cmd_buffer[CMD_BUFFER];
    ClientHandler handler(data_buffer, cmd_buffer);
    handler.Initialise();
    SOCKET socket_descriptor;
    struct sockaddr_in serveraddress;

    WSADATA WSAData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &WSAData) != 0)
        return 0;

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
    int iTimeout = 5000;
    auto iRet = setsockopt(socket_descriptor,
                           SOL_SOCKET,
                           SO_RCVTIMEO,
                           /*
                           reinterpret_cast<char*>(&tv),
                           sizeof(timeval) );
                           */
                           (const char *)&iTimeout,
                           sizeof(iTimeout));
    while (handler.getWork())
    {
        handler.Run();
        if (!handler.getWork())
            return;
        // отправляем пакет команд
        int iResult = sendto(socket_descriptor, cmd_buffer, CMD_BUFFER, 0, nullptr, sizeof(serveraddress));
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(socket_descriptor);
            WSACleanup();
            return 1;
        }

        // получение команды от сервера
        recvfrom(socket_descriptor, cmd_buffer, sizeof(cmd_buffer), 0, nullptr, nullptr);

        // получение данных от сервера
        recvfrom(socket_descriptor, data_buffer, sizeof(data_buffer), 0, nullptr, nullptr);
    }

    // cleanup
    closesocket(socket_descriptor);
    WSACleanup();
    system("pause");
    return 0;
}
#endif