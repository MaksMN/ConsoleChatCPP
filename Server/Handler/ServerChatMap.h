#pragma once
#include <string>
#include <cstring>
#include <memory>
#include <vector>
#include <map>
#include <set>

#define MAIN_PAGE "MAIN_PAGE"
#define PUBLIC_PAGE "PUBLIC_PAGE"
#define PUBLIC_PAGE_INPUT "PUBLIC_PAGE_INPUT"
#define GUEST_PAGE "GUEST_PAGE"
#define INPUT_LOGIN_PAGE "INPUT_LOGIN_PAGE"
#define INPUT_REGISTRATION_PAGE "INPUT_REGISTRATION_PAGE"
#define PRIVATE_PAGE_USERS "PRIVATE_PAGE_USERS"
#define PRIVATE_PAGE_USERS_INPUT "PRIVATE_PAGE_USERS_INPUT"
#define PRIVATE_PAGE_MESSAGES "PRIVATE_PAGE_MESSAGES"
#define PRIVATE_PAGE_MESSAGES_INPUT "PRIVATE_PAGE_INPUT"

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
#define NONE "NONE"
#define ADMIN "/admin"
#define UPDATE "/update"
#define PM "/pm"

class ServerChatMap
{
private:
    std::map<std::string, std::set<std::string>> pages;

public:
    ~ServerChatMap() = default;
    ServerChatMap();

    bool checkPage(std::string &page, std::string &command);
    bool checkPage(std::string &&page, std::string &&command);

    bool checkPage(std::string &&page, std::string &command);
};
