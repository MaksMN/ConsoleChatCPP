#pragma once
#include "../Misc/Misc.h"
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
#include "../Misc/UserInputW.h"
#else
#include "../Misc/UserInput.h"
#endif
#include <string>

#define DATA_BUFFER 4096 // Размер буфера для данных
#define CMD_BUFFER 1024  // Размер буфера команд и заголовков

typedef unsigned int uint;
typedef unsigned long long ullong;
const extern char clear[];
class ClientHandler
{
private:
    char (&data_buffer)[DATA_BUFFER];
    char (&cmd_buffer)[CMD_BUFFER];

    std::string pageText = "MAIN_PAGE";
    uint pagePos;
    uint pageSize = 9;

    std::string dataText;

    std::string cmdText = "HELLO";
    uint cmdPos;
    uint cmdSize = 5;

    ullong session_key = 0;
    uint login_size = 5;
    std::string login = "Guest";

    UserInput<int, int> userInputInt;
    UserInput<std::string, std::string> userInputStr;

public:
    ClientHandler(char (&_data_buffer)[DATA_BUFFER], char (&_cmd_buffer)[CMD_BUFFER]);
    ~ClientHandler() = default;
    void Initialise();
    void Run();
};
