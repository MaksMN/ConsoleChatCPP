#pragma once
#include "../Misc/Misc.h"
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
#include "../Misc/UserInputW.h"
#else
#include "../Misc/UserInput.h"
#endif

#include <string>
typedef unsigned int uint;
typedef unsigned long long ullong;
const extern char clear[];
class ClientHandler
{
private:
    ullong session_key = 0;
    std::string login;

public:
    std::vector<char> in_buffer;
    ~ClientHandler() = default;

    void Run();
};
