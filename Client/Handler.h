#pragma once
#include "../Misc/Misc.h"

#if defined(_WIN64) || defined(_WIN32)
#include "../Misc/UserInputW.h"
#else
#include "../Misc/UserInput.h"
#endif

#include <string>
typedef unsigned int uint;
typedef unsigned long long ullong;

class Handler
{
private:
    ullong session_key = 0;
    std::string login;

    char (&_buffer)[];

public:
    Handler(char (&_buffer)[], uint size);
    ~Handler();
};
