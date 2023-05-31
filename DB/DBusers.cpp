#include "DBusers.h"

bool DBusers::uniqueLogin(std::wstring &login) const
{
    if (_DB.empty())
        return true;
    return !std::any_of(_DB.begin(), _DB.end(), [&login](const auto &u)
                        { return u->getLogin() == login; });
}

std::shared_ptr<User> DBusers::addUser(const std::wstring &login, const std::wstring &name, std::wstring &pass)
{
    _DB.push_back(std::make_shared<User>(lastUserID++, login, name, pass));
    return _DB.back();
}

std::shared_ptr<User> DBusers::addUser(const std::wstring &&login, const std::wstring &&name, std::wstring &&pass)
{
    return addUser(login, name, pass);
}

std::shared_ptr<User> DBusers::getUserByID(uint id)
{
    return this->operator[](id);
}

std::shared_ptr<User> DBusers::getUserByLogin(std::wstring &login)
{
    auto it = std::find_if(_DB.begin(), _DB.end(),
                           [&login](const auto &u)
                           {
                               return u->getLogin() == login;
                           });

    if (_DB.end() != it)
        return *it;
    return nullptr;
}
