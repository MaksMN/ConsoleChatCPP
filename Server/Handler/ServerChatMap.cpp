#include "ServerChatMap.h"

ServerChatMap::ServerChatMap()
{
    // команды для конкретных страниц
    pages[MAIN_PAGE].insert(NONE);

    pages[GUEST_PAGE].insert(LOGIN);
    pages[GUEST_PAGE].insert(REG);

    pages[INPUT_LOGIN_PAGE].insert(ALL);
    pages[INPUT_REGISTRATION_PAGE].insert(ALL);

    pages[PUBLIC_PAGE];
    pages[PUBLIC_PAGE_INPUT].insert(ALL);

    pages[PRIVATE_PAGE];
    pages[PRIVATE_PAGE_INPUT].insert(ALL);

    // команды для всех страниц
    for (auto it = pages.begin(); it != pages.end(); it++)
    {
        pages[it->first].insert(LOGOUT);
        pages[it->first].insert(HELP);
        pages[it->first].insert(SV_QUIT);
        pages[it->first].insert(HOME);
        pages[it->first].insert(HELLO);
        pages[it->first].insert(CHAT);
        pages[it->first].insert(ADMIN);
    }
}

bool ServerChatMap::checkPage(std::string &page, std::string &command)
{
    if (!pages.contains(page))
        return false;
    return pages[page].contains(command) || pages[page].contains(ALL);
}

bool ServerChatMap::checkPage(std::string &&page, std::string &&command)
{
    return checkPage(page, command);
}

bool ServerChatMap::checkPage(std::string &&page, std::string &command)
{
    return checkPage(page, command);
}
