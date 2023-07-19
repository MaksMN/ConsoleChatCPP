#include "ServerChatMap.h"

ServerChatMap::ServerChatMap()
{
    // команды для конкретных страниц
    pages[MAIN_PAGE].insert(NONE);

    pages[GUEST_PAGE].insert(LOGIN);
    pages[GUEST_PAGE].insert(REG);

    pages[INPUT_LOGIN_PAGE].insert(ALL);
    pages[INPUT_REGISTRATION_PAGE].insert(ALL);

    pages[PUBLIC_PAGE].insert(ALL);
    pages[PUBLIC_PAGE_INPUT].insert(ALL);

    pages[PRIVATE_PAGE_USERS].insert(PM);
    pages[PRIVATE_PAGE_USERS].insert(USERS);
    pages[PRIVATE_PAGE_USERS_INPUT].insert(ALL);

    pages[PRIVATE_PAGE_MESSAGES].insert(ALL);
    pages[PRIVATE_PAGE_MESSAGES_INPUT].insert(ALL);

    pages[PROFILE_PAGE].insert(ALL);
    pages[PROFILE_PAGE_INPUT].insert(ALL);

    pages[USER_BANNED].insert(ALL);

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
        pages[it->first].insert(UPDATE);
    }
}

bool ServerChatMap::checkPage(const std::string &page, const std::string &command)
{
    if (!pages.contains(page))
        return false;
    return pages[page].contains(command) || pages[page].contains(ALL);
}

bool ServerChatMap::checkPage(const std::string &&page, const std::string &&command)
{
    return checkPage(page, command);
}

bool ServerChatMap::checkPage(const std::string &&page, const std::string &command)
{
    return checkPage(page, command);
}
