#include "WinServer.h"
#if defined(_WIN64) || defined(_WIN32)
int server_socket(char port[])
{
    char cmd_buffer[CMD_BUFFER];
    char data_buffer[DATA_BUFFER];
    ServerHandler handler(data_buffer, cmd_buffer);
    handler.InitialiseDB();

    WSADATA WSAData;
    WORD sockVersion = MAKEWORD(2, 2);
    if (WSAStartup(sockVersion, &WSAData) != 0)
        return 0;

    SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Create server socket
    if (INVALID_SOCKET == serSocket)
    {
        std::cout << "socket error!";
        return 0;
    }

    // Set the transmission protocol, port and destination address
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) // bind socket to address
    {
        std::cout << "bind error";
        closesocket(serSocket);
        return 0;
    }

    sockaddr_in clientAddr;
    int iAddrlen = sizeof(clientAddr);

    do
    {
        // ждем запросы от клиентов
        int iResult = recvfrom(serSocket, cmd_buffer, CMD_BUFFER, 0, (sockaddr *)&clientAddr, &iAddrlen);
        if (iResult > 0)
        {
            printf("Bytes received: %d\n", iResult);

            // обрабатываем взводящие данные
            handler.Run();

            // отправляем клиенту буфер команд
            int iSendResult = sendto(serSocket, cmd_buffer, CMD_BUFFER, 0, (sockaddr *)&clientAddr, iAddrlen);

            if (iSendResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(serSocket);
                WSACleanup();
                return 1;
            }
            // Отправляем клиенту буфер данных
            iSendResult = sendto(serSocket, data_buffer, DATA_BUFFER, 0, (sockaddr *)&clientAddr, iAddrlen);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(serSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(serSocket);
            WSACleanup();
            return 1;
        }

    } while (1);

    // cleanup
    closesocket(serSocket);
    WSACleanup();
    system("pause");
    return 0;
}

#endif