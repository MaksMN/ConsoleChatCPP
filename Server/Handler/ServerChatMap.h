#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include <map>
#include <set>

#define MAIN_PAGE "MAIN_PAGE"

#define ALL "ALL"
#define CHAT "/chat"
#define HELLO "/hello"
#define HELP "/help"
#define HOME "/home"
#define LOGIN "/login"
#define LOGOUT "/logout"
#define QUIT "/quit"
#define REG "/reg"
#define SV_QUIT "/sv_quit"

class ServerChatMap
{
private:
    std::map<std::string, std::set<std::string>> pages;

public:
    ~ServerChatMap() = default;
    ServerChatMap();

    bool checkPage(std::string &page, std::string &command);
};
