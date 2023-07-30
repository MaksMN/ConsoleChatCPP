#include "Message.h"

Message::Message(ullong author_id, std::string text)
{
    _author_id = author_id;
    _text = text;
    _status = msg::public_;
    _published = time(NULL);
}

Message::Message(ullong id, ullong author_id, std::string text, ullong published, uint status)
{
    _id = id;
    _author_id = author_id;
    _text = text;
    _published = published;
    _status = (msg::status)status;
}

Message::Message(ullong author_id, ullong recipient_id, std::string text)
{
    _author_id = author_id;
    _recipient_id = recipient_id;
    _text = text;
    _status = msg::private_;
    _published = time(NULL);
}

Message::Message(ullong id, ullong author_id, ullong recipient_id, std::string text, ullong published, uint status)
{
    _id = id;
    _author_id = author_id;
    _recipient_id = recipient_id;
    _text = text;
    _published = published;
    _status = (msg::status)status;
}

msg::status Message::getStatus()
{
    return _status;
}

uint Message::getStatusInt()
{
    return (uint)_status;
}

void Message::setStatus(msg::status status)
{
    _status = status;
}

bool Message::isPublic()
{
    return flags.hasFlag(msg::public_, getStatus());
}

bool Message::isPrivate()
{
    return flags.hasFlag(msg::private_, getStatus());
}

void Message::toPublic()
{
    flags.removeFlag(msg::private_, _status);
    flags.addFlag(msg::public_, _status);
}

void Message::toPrivate()
{
    flags.removeFlag(msg::public_, _status);
    flags.addFlag(msg::private_, _status);
}

void Message::toDelivered()
{
    flags.addFlag(msg::message_delivered, _status);
}

void Message::unDelivered()
{
    flags.removeFlag(msg::message_delivered, _status);
}

bool Message::isDelivered()
{
    return flags.hasFlag(msg::message_delivered, _status);
}

void Message::read()
{
    flags.addFlag(msg::message_read, _status);
}

void Message::toUnread()
{
    flags.removeFlag(msg::message_read, _status);
}

bool Message::isRead()
{
    return flags.hasFlag(msg::message_read, _status);
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

uint Message::getOwnerID()
{
    return _author_id;
}
