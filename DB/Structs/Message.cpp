#include "Message.h"

Message::Message(const uint &id,
                 const uint &author_id,
                 const uint &recipient_id,
                 const std::string &text,
                 msg::status status)
    : _id(id),
      _author_id(author_id),
      _recipient_id(recipient_id),
      _status(status),
      _text(text),
      _timestamp(time(NULL)) {}

msg::status Message::getStatus()
{
    return _status;
}

void Message::setStatus(msg::status status)
{
    _status = status;
}

void Message::hide()
{
    _status = flags.addFlag(_status, msg::hidden_);
}

bool Message::isHidden()
{
    return flags.hasFlag(_status, msg::hidden_);
}

void Message::show()
{
    _status = flags.removeFlag(_status, msg::hidden_);
}

void Message::to_public()
{
    _status = flags.flagsReplace(_status, msg::public_, msg::private_);
}

void Message::to_private()
{
    _status = flags.flagsReplace(_status, msg::private_, msg::public_);
}

uint Message::getID()
{
    return _id;
}

uint Message::getAuthorID()
{
    return _author_id;
}

uint Message::getRecipientID()
{
    return _recipient_id;
}

std::string Message::getText()
{
    return _text;
}

void Message::printData()
{
    std::string id = flags.hasFlag(_status, msg::complain_) ? "complaint id" : "msgid";
    std::cout << "Инфо: [" << id << " " << getID() << "] Опубликовано: " << StampToTime(_timestamp) << std::endl;
    std::cout << "Текст: ";
    if (isHidden())
    {
        std::cout << "Сообщение скрыто администратором";
    }
    else
    {
        std::cout << _text << std::endl;
    }
    std::cout << std::endl;
}
