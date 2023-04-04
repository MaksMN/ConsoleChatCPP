//
// DB.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "DB.h"
#include "EncodePassword.h"

bool DB::addUser(const std::wstring &name,
                 const std::wstring &login,
                 const std::wstring &pass)
{
    if (isUniqueLogin(login))
    {
        User user(name, login);
        user.setUserPassword(pass);
        user.setCurrentID();
        int countMessages = addMessagesToNewUserFromPublicChat(user.getId());
        user.addedMessage(countMessages);
        _userDB.append(user);
        return true;
    }
    return false;
}

bool DB::addUser(User &user)
{
    if (isUniqueLogin(user.getUserLogin()))
    {
        if (user.getId() == 0)
        {
            user.setCurrentID();
        }
        int countMessages = addMessagesToNewUserFromPublicChat(user.getId());
        user.addedMessage(countMessages);
        _userDB.append(user);

        return true;
    }
    return false;
}

void DB::addMessage(Message &message)
{
    if (message.getAuthorID() != 0 && message.getRecipientID() != 0)
    {
        message.isPrivate = true;
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() == message.getAuthorID())
            {
                _userDB[i].addedMessage();
            }

            if (_userDB[i].getId() == message.getRecipientID())
            {
                _userDB[i].addedMessage();
            }
        }
        message.setDateMessage();
        message.setMessageId();
        _messageDB.append(message);
    }
}

void DB::AddMessageToAllUsers(Message &message)
{
    if (message.getAuthorID() != 0)
    {
        message.isPrivate = false;
        message.setDateMessage();
        message.setMessageId();
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() != message.getAuthorID())
            {
                message.setRecipientID(_userDB[i].getId());
            }
            else
            {
                message.setRecipientID(0);
            }
            _userDB[i].addedMessage();
            _messageDB.append(message);
        }
    }
}

bool DB::setUserPassword(int userID, const std::wstring &pass)
{

    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getId() == userID)
        {
            _userDB[i].setUserPassword(pass);
            return true;
        }
    }
    return false;
}

bool DB::isUniqueLogin(const std::wstring &login)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getUserLogin() == login)
        {
            return false;
        }
    }
    return true;
}

bool DB::isCorrectPassword(int userID, const std::wstring &pass)
{
    for (int i = 0; i < _userDB.count(); i++)
    {
        if (_userDB[i].getId() == userID)
        {
            std::wstring verifyPass = pass;
            EncodePassword::encodePassword(verifyPass);
            if (_userDB[i].getUserPassword() == verifyPass)
            {
                return true;
            }
        }
    }
    return false;
}

const std::unique_ptr<User[]> DB::getAllUsers() const
{
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User[]> ret(new User[_userDB.count()]);
        for (int i = 0; i < _userDB.count(); i++)
        {
            ret[i] = _userDB[i];
        }
        return ret;
    }
    return nullptr;
}

const std::unique_ptr<User[]> DB::getAllUsers(const std::wstring &name, int &size) const
{
    size = 0;
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User[]> ret(new User[_userDB.count()]);
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getUserName() == name)
            {
                ret[i] = _userDB[i];
                size++;
            }
        }
        if (size > 0)
        {
            return ret;
        }
    }
    size = 0;
    return nullptr;
}

const std::unique_ptr<User> DB::getUserByLogin(const std::wstring &login, bool exception) const
{
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User> user(new User());
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getUserLogin() == login)
            {
                (*user) = _userDB[i];
                return user;
            }
        }
    }
    if (exception)
    {
        throw UserNotFoundException();
    }
    return nullptr;
}

const std::unique_ptr<User> DB::getUserById(const int &userID) const
{
    if (_userDB.count() > 0)
    {
        std::unique_ptr<User> user(new User());
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() == userID)
            {
                (*user) = _userDB[i];
                return user;
            }
        }
    }
    return nullptr;
}

int DB::usersCount() const
{
    return _userDB.count();
}

const std::unique_ptr<Message[]> DB::getAllMessages() const
{
    if (_messageDB.count() > 0)
    {
        std::unique_ptr<Message[]> ret(new Message[_messageDB.count()]);
        for (int i = 0; i < _messageDB.count(); i++)
        {
            ret[i] = _messageDB[i];
        }
        return ret;
    }
    return nullptr;
}

const std::unique_ptr<Message> DB::getMessage(const int &messageId) const
{
    if (_messageDB.count() > 0)
    {
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (_messageDB[i].getId() == messageId)
            {
                return std::make_unique<Message>(_messageDB[i]);
            }
        }
    }
    return nullptr;
}

const std::unique_ptr<Message[]> DB::getAllMessagesForUserById(int id) const
{
    DBCore<Message> newMessageArray;

    if (_messageDB.count() > 0)
    {
        bool issetMessage = false;
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if ((_messageDB[i].getAuthorID() == id) || (_messageDB[i].getRecipientID() == id))
            {
                newMessageArray.append(_messageDB[i]);
                issetMessage = true;
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }
            return ret;
        }
    }
    return nullptr;
}

bool DB::updateUserData(const User &user)
{
    if (_userDB.count() > 0)
    {
        for (int i = 0; i < _userDB.count(); i++)
        {
            if (_userDB[i].getId() == user.getId())
            {
                _userDB[i].setUserName(user.getUserName());
                _userDB[i].setUserLogin(user.getUserLogin());
                _userDB[i].setIsBanned(user.isBanned());
                _userDB[i].setIsAdmin(user.isAdmin());
                return true;
            }
        }
    }
    return false;
}

bool DB::deleteUserAccount(User &user)
{
    if (_userDB.count() > 0 && !(isUniqueLogin(user.getUserLogin())))
    {
        if (user.isDeleted())
        {
            return true;
        }
        user.deleteThisData();
        _userDB.operator--(user.getId());
        return true;
    }
    return false;
}

bool DB::deleteMessageById(int Id)
{
    if (_messageDB.count() > 0)
    {
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (_messageDB[i].getId() == Id)
            {
                int authorId = _messageDB[i].getAuthorID();
                int recipientId = _messageDB[i].getRecipientID();

                for (int index = 0; index < _userDB.count(); index++)
                {
                    if (_userDB[index].getId() == authorId || _userDB[index].getId() == recipientId)
                    {
                        _userDB[index].deletedMessage();
                    }
                }
            }
        }
        _messageDB.deleteById(Id);
        return true;
    }
    return false;
}

// Private methods
bool DB::isUsedId(std::unique_ptr<int[]> const &arrayId, int id, int size) const
{
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            if (arrayId[i] == id)
            {
                return true;
            }
        }
    }
    return false;
}

int DB::addMessagesToNewUserFromPublicChat(int id)
{
    int count{0};
    if (_messageDB.count() > 0)
    {
        std::unique_ptr<int[]> arrayId = std::make_unique<int[]>(_messageDB.count());
        int messageCount = _messageDB.count();
        Message message;

        for (int i = 0; i < messageCount; i++)
        {
            if (!(_messageDB[i].isPrivate))
            {
                if (_messageDB[i].getRecipientID() != id && _messageDB[i].getRecipientID() != 0 && !isUsedId(arrayId, _messageDB[i].getId(), count))
                {
                    message = _messageDB[i];
                    message.setRecipientID(id);
                    _messageDB.append(message);
                    arrayId[count] = message.getId();
                    count++;
                }
            }
        }
    }
    return count;
}

const std::unique_ptr<Message[]> DB::getAllPrivateMessagesForUsersById(int user1Id, int user2Id, int &size) const
{
    if (_messageDB.count() > 0)
    {
        DBCore<Message> newMessageArray;
        bool issetMessage = false;

        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (((_messageDB[i].getAuthorID() == user1Id) && (_messageDB[i].getRecipientID() == user2Id)) || ((_messageDB[i].getAuthorID() == user2Id) && (_messageDB[i].getRecipientID() == user1Id)))
            {
                if (_messageDB[i].isPrivate)
                {
                    newMessageArray.append(_messageDB[i]);
                    issetMessage = true;
                }
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }
            size = newMessageArray.count();
            return ret;
        }
    }
    size = 0;
    return nullptr;
}

const std::unique_ptr<Message[]> DB::getAllPrivateMessagesForUserById(int userId, int &size) const
{
    DBCore<Message> newMessageArray;

    if (_messageDB.count() > 0)
    {
        bool issetMessage = false;
        for (int i = 0; i < _messageDB.count(); i++)
        {
            if ((_messageDB[i].getAuthorID() == userId) || (_messageDB[i].getRecipientID() == userId))
            {
                if (_messageDB[i].isPrivate)
                {
                    newMessageArray.append(_messageDB[i]);
                    issetMessage = true;
                }
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }
            size = newMessageArray.count();
            return ret;
        }
    }
    size = 0;
    return nullptr;
}

const std::unique_ptr<Message[]> DB::getAllPublicMessagesForUserById(int Id, int &size) const
{

    if (_messageDB.count() > 0)
    {
        DBCore<Message> newMessageArray;
        bool issetMessage = false;
        int count{0};
        std::unique_ptr<int[]> arrayId = std::make_unique<int[]>(_messageDB.count());

        for (int i = 0; i < _messageDB.count(); i++)
        {
            if ((_messageDB[i].getAuthorID() == Id) || (_messageDB[i].getRecipientID() == Id))
            {
                if (!_messageDB[i].isPrivate && !isUsedId(arrayId, _messageDB[i].getId(), count))
                {
                    newMessageArray.append(_messageDB[i]);
                    issetMessage = true;
                    arrayId[count] = _messageDB[i].getId();
                    count++;
                }
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }
            size = newMessageArray.count();
            return ret;
        }
    }
    size = 0;
    return nullptr;
}

const std::unique_ptr<Message[]> DB::getAllPublicMessages(int &size) const
{

    if (_messageDB.count() > 0)
    {
        DBCore<Message> newMessageArray;
        bool issetMessage = false;
        int count{0};
        std::unique_ptr<int[]> arrayId = std::make_unique<int[]>(_messageDB.count());

        for (int i = 0; i < _messageDB.count(); i++)
        {
            if (!_messageDB[i].isPrivate && !isUsedId(arrayId, _messageDB[i].getId(), count))
            {
                newMessageArray.append(_messageDB[i]);
                issetMessage = true;
                arrayId[count] = _messageDB[i].getId();
                count++;
            }
        }
        if (issetMessage)
        {
            std::unique_ptr<Message[]> ret(new Message[newMessageArray.count()]);
            for (int i = 0; i < newMessageArray.count(); i++)
            {
                ret[i] = newMessageArray[i];
            }
            size = newMessageArray.count();
            return ret;
        }
    }
    size = 0;
    return nullptr;
}