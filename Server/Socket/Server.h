#pragma once
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "../Handler/Handler.h"
#include "../../Misc/Misc.h"

#define BUFFER 4096 // Максимальный размер буфера для приема и передачи
const extern int PORT;

void processRequest();
