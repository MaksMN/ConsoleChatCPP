#include "Message.h"

Message::Message(const uint &id,
                 const uint &author_id,
                 const uint &recipient_id,
                 const std::string &text,
                 msg::status status,
                 const std::string &db_file)
    : _id(id),
      _author_id(author_id),
      _recipient_id(recipient_id),
      _status(status),
      _text(text),
      _timestamp(time(NULL)),
      DBfilePath(db_file) {}

Message::Message(std::ifstream &stream, const std::string &db_file)
    : _id(Stream::getUint(stream, 4)),
      _author_id(Stream::getUint(stream, 12)),
      _recipient_id(Stream::getUint(stream, 16)),
      _status((msg::status)Stream::getUint(stream, 8)),
      _text(Stream::getString(stream, 28)),
      _timestamp(Stream::getLong64(stream, 20)),
      DBfilePath(db_file)
{
    /*
    |block_size|  _id    |   status  |  author  |  recipient   |  timestamp |  text size | text  |
               4         8           12         16             20           28           32
    */

    auto pos = stream.tellg();
    uint block_size = Stream::getUint(stream);
    stream.seekg(pos += (block_size + 4));
}

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
    std::cout << "Инфо: [" << id << " " << getID() << "] Опубликовано: " << Misc::StampToTime(_timestamp) << std::endl;
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

std::string Message::getData()
{
    return "Инфо: [msgid " + std::to_string(getID()) + "] Опубликовано: " + Misc::StampToTime(_timestamp) + "\n"
                                                                                                            "Текст: " +
           (isHidden() ? "Сообщение скрыто администратором" : _text);
}

void Message::writeData()
{
    /*
    |block_size|  _id    |   status  |  author  |  recipient   |  timestamp |  text size | text  |
               4         8           12         16             20           28           32
    */
    std::ofstream stream(DBfilePath, std::ios::app | std::ios::ate | std::ios::binary);
    stream << "MESG";
    const uint uintSize = sizeof(uint);
    const uint longSize = sizeof(unsigned long long);
    const uint textSize = _text.size();
    const uint status = (int)_status;

    const uint block_size = 32 - 4 + textSize;

    char uint_num[uintSize];
    char long_num[longSize];

    // block_size
    memcpy(uint_num, &block_size, uintSize);
    stream.write(uint_num, uintSize);

    // id
    memcpy(uint_num, &_id, uintSize);
    stream.write(uint_num, uintSize);

    // status
    memcpy(uint_num, &status, uintSize);
    stream.write(uint_num, uintSize);

    // author_id
    memcpy(uint_num, &_author_id, uintSize);
    stream.write(uint_num, uintSize);

    // recipient_id
    memcpy(uint_num, &_recipient_id, uintSize);
    stream.write(uint_num, uintSize);

    // timestamp
    memcpy(long_num, &_timestamp, longSize);
    stream.write(long_num, longSize);

    // text
    memcpy(uint_num, &textSize, uintSize);
    stream.write(uint_num, uintSize);
    stream << _text;
    stream.close();
    std::filesystem::permissions(
        DBfilePath,
        std::filesystem::perms::owner_all & ~(std::filesystem::perms::group_all |
                                              std::filesystem::perms::others_all));
}

uint Message::getOwnerID()
{
    return _author_id;
}
