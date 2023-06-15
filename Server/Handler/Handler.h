#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include "../../Misc/Misc.h"
#include "../../DB/Headers.h"

#define BUFFER 4096

typedef unsigned int uint;
typedef unsigned long long ullong;
class Handler
{
private:
    char (&_buffer)[BUFFER];

    std::string cmdText;
    uint cmdPos;

    ulong session_key;
    uint login_size;
    std::string login;

    DBmessages pubMessagesDB{"pub_messages"};
    DBmessages privMessagesDB{"priv_messages"};
    DBcomplaints complaintsDB;
    DBusers usersDB{"users"};

public:
    std::vector<char> message;
    Handler(char (&buffer)[BUFFER]);
    ~Handler() = default;
    void InitialiseDB();
    void Run();
};