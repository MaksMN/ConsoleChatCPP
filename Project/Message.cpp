//
// Message.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "Message.h"
#include <chrono>

long long int Message::_currentId;

void Message::setMessage(const std::wstring &message)
{
    _message = message;
}

void Message::setAuthorID(int authorID)
{
    _authorID = authorID;
}

void Message::setRecipientID(int recipientID)
{
    _recipientID = recipientID;
}

const std::wstring &Message::getMessage() const
{
    return _message;
}

int Message::getAuthorID() const
{
    return _authorID;
}

int Message::getRecipientID() const
{
    return _recipientID;
}

long long int Message::getId() const
{
    return _id;
}

long long int Message::getDate() const
{
    return _date;
}

Message &Message::operator=(const Message &message)
{
    isPrivate = message.isPrivate;
    _message = message.getMessage();
    _authorID = message.getAuthorID();
    _recipientID = message.getRecipientID();
    _date = message.getDate();
    _id = message.getId();
    return *this;
}

// Private methods for Friends L"DB"
void Message::setDateMessage()
{
    using namespace std::chrono;
    long long int sec = int(duration_cast<seconds>(system_clock::now().time_since_epoch()).count());
    _date = sec;
}

void Message::setMessageId()
{
    _currentId++;
    _id = _currentId;
}
