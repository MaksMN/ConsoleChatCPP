#include "ServerChatMap.h"

ServerChatMap::ServerChatMap()
{
    pages.insert("/chat");
    pages.insert("/private");
    pages.insert("/users");
    pages.insert("/profile");
}

std::string ServerChatMap::check(std::string page, std::string command)
{
    if (pages.contains(command))
        return command;
    if (pages.contains(page))
        return page;
    return "none";
}
