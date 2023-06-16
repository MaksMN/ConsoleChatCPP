#pragma once
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <sys/socket.h> // Библиотека для работы с сокетами
#include <arpa/inet.h>
#include "../Misc/Misc.h"
#include "ClientHandler.h"

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков
const extern int PORT;

void sendRequest();