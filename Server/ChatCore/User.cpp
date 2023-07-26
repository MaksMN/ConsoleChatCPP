#include "User.h"

User::User(const ullong &id, const std::string &login, const std::string &name, std::string &pass)
    : _id(id),
      _login(login),
      _name(name),
      _timestamp(time(NULL))
{
    setPass(pass);
    _status = user::user_;
}

User::User(const ullong &&id, const std::string &&login, const std::string &&name, std::string &&pass)
    : _id(id),
      _login(login),
      _name(name),
      _timestamp(time(NULL))
{
    setPass(pass);
    _status = user::user_;
}

uint User::getID()
{
    return _id;
}

std::string User::getName()
{
    return _name;
}
void User::setName(const std::string &name)
{
    _name = name;
}

std::string User::getLogin()
{
    return _login;
}
user::status User::getStatus()
{
    return _status;
}
void User::setStatus(user::status &status)
{
    if (getID() == 0)
    {
        return;
    }
    _status = status;
}

void User::toUser()
{
    if (getID() == 0)
    {
        return;
    }
    _status = flags.flagsReplace(_status, user::status::user_, user::status::admin_);
}

bool User::isBanned()
{
    return flags.hasFlag(_status, user::banned_);
}
void User::unBan()
{
    _status = flags.removeFlag(_status, user::banned_);
}
void User::ban()
{
    if (getID() == 0)
    {
        return;
    }
    toUser();
    _status = flags.addFlag(_status, user::banned_);
}

bool User::isAdmin()
{
    return flags.hasFlag(_status, user::admin_);
}
bool User::isUser()
{
    return flags.hasFlag(_status, user::user_);
}
void User::toAdmin()
{
    _status = flags.flagsReplace(_status, user::admin_, user::user_ | user::banned_);
}

void User::printData()
{
    std::cout << _name << "[" << _login << "] [userid " << _id << "] ";
    if (isAdmin())
        std::cout << "[group admin] ";
    if (isBanned())
        std::cout << "[status banned] ";
    if (isUser())
        std::cout << "[group user] ";

    std::cout << "Рег." << Misc::StampToTime(_timestamp) << std::endl;
    std::cout << std::endl;
}

std::string User::getData()
{
    std::string s1 =
        _name + "[" + _login + "] [userid " + std::to_string(_id) + "] ";

    if (isAdmin())
        s1 += "[group admin] ";
    if (isBanned())
        s1 += "[status banned] ";
    if (isUser())
        s1 += "[group user] ";
    s1 += "Рег." + Misc::StampToTime(_timestamp) + "\n";
    return s1;
}

void User::setPass(std::string &pass)
{
    pass_salt = Misc::getRandomStr(40);
    pass_hash = sha1.hash(pass + pass_salt);
    for (int i{0}; i < pass.size(); i++)
        pass.data()[i] = '\0';
}

ullong User::getSessionKey()
{
    return session_key;
}

void User::setSessionKey(ullong key)
{
    session_key = key;
}

uint User::getOwnerID()
{
    return getID();
}
