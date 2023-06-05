#include "DBusers.h"

bool DBusers::uniqueLogin(std::string &login) const
{
    if (_DB.empty())
        return true;
    return !std::any_of(_DB.begin(), _DB.end(), [&login](const auto &u)
                        { return u->getLogin() == login; });
}

std::shared_ptr<User> DBusers::addUser(const std::string &login, const std::string &name, std::string &pass)
{
    _DB.push_back(std::make_shared<User>(lastUserID++, login, name, pass));
    _DB.back()->writeData();
    return _DB.back();
}

std::shared_ptr<User> DBusers::addUser(const std::string &&login, const std::string &&name, std::string &&pass)
{
    return addUser(login, name, pass);
}

std::shared_ptr<User> DBusers::getUserByID(uint id)
{
    return this->operator[](id);
}

std::shared_ptr<User> DBusers::getUserByLogin(std::string &login)
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

void DBusers::updateFromFile()
{
    std::ifstream stream("users", std::ios::app | std::ios::ate | std::ios::binary);
    stream.seekg(0);

    while (!stream.eof())
    {
        char u[4];
        stream.read(u, 4);
        auto pos = stream.tellg();
        std::string u_(u, 4);
        if (u_ != "USER" || pos == -1)
            break;

        _DB.push_back(std::make_shared<User>(stream));
    }
    stream.close();
    lastUserID = _DB.back()->getID();
}
