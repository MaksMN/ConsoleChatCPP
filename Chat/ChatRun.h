#pragma once
#include "Admin/AdminInterface.h"
#include "Users/UserInterface.h"
#include "../DB/DBmessages.h"
#include "../DB/DBusers.h"
#include "../DB/DBcomplaints.h"
#include "../Misc/UserInput.h"
#include "Options.h"

/// @brief Класс для подготовки и запуска чата.
class ChatRun
{
private:
    DBmessages pubMessagesDB{"pub_messages"};
    DBmessages privMessagesDB{"priv_messages"};
    DBcomplaints complaintsDB;
    DBusers usersDB{"users"};

    UserInterface userInterface{pubMessagesDB, privMessagesDB, complaintsDB, usersDB};
    AdminInterface adminInterface{pubMessagesDB, privMessagesDB, complaintsDB, usersDB};

public:
    ~ChatRun() = default;
    void Run();
};