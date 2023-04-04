//
// User.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "User.h"
#include "DB.h"
#include "EncodePassword.h"
long long int User::_currentId;

// Public methods

User::User(const std::wstring &name, const std::wstring &login, const std::wstring &pass) : _name(name), _login(login)
{
    setUserPassword(pass);
    setCurrentID();
}

long long int User::getId() const
{
    return _id;
}

const std::wstring &User::getUserName() const
{
    return _name;
}

const std::wstring &User::getUserLogin() const
{
    return _login;
}

const std::wstring &User::getUserPassword() const
{
    return _pass;
}

void User::setUserName(const std::wstring &name)
{
    _name = name;
}

void User::setUserLogin(const std::wstring &login)
{
    _login = login;
}

int User::getMessagesCount() const
{
    return _messageCount;
}

bool User::isAdmin() const
{
    return _isAdmin;
}

bool User::isBanned() const
{
    return _isBanned;
}

bool User::isDeleted() const
{
    return _isDeleted;
}

void User::setIsAdmin(bool isAdmin)
{
    _isAdmin = isAdmin;
}

void User::setIsBanned(bool isBanned)
{
    _isBanned = isBanned;
}

void User::deleteThisData()
{
    _name = L"deleted";
    std::wstring pass = L"DeLeTeD";
    EncodePassword::encodePassword(pass);
    _pass = pass;
    _isAdmin = false;
    _isBanned = false;
    _isDeleted = true;
}

User &User::operator=(const User &user)
{
    _name = user._name;
    _login = user._login;
    _pass = user._pass;
    _messageCount = user._messageCount;
    _id = user._id;
    _isAdmin = user._isAdmin;
    _isBanned = user._isBanned;
    _isDeleted = user._isDeleted;
    return *this;
}

// Private methods for Friends L"DB"

void User::setUserPassword(const std::wstring &pass)
{
    std::wstring passWord = pass;
    EncodePassword::encodePassword(passWord);
    _pass = passWord;
}

void User::copyUserPassword(const std::wstring &pass)
{
    _pass = pass;
}

void User::setMessageCout(int cout)
{
    _messageCount = cout;
}

void User::addedMessage(int count)
{
    _messageCount = _messageCount + count;
}

void User::setCurrentID()
{
    _currentId++;
    _id = _currentId;
}

void User::setUserID(int id)
{
    _id = id;
}

void User::deletedMessage()
{
    if (_messageCount > 0)
    {
        _messageCount--;
    }
}