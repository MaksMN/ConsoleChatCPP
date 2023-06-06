#include "User.h"

User::User(const uint &id, const std::string &login, const std::string &name, std::string &pass, std::string db_file)
    : _id(id),
      _login(login),
      _name(name),
      _timestamp(time(NULL)),
      DBfilePath(db_file)
{
    setPass(pass);
    _status = user::user_;
}

User::User(const uint &&id, const std::string &&login, const std::string &&name, std::string &&pass, std::string db_file)
    : _id(id),
      _login(login),
      _name(name),
      _timestamp(time(NULL)),
      DBfilePath(db_file)
{
    setPass(pass);
    _status = user::user_;
}

User::User(std::ifstream &stream, std::string db_file)
    : _id(Stream::getUint(stream, 4)),
      _login(Stream::getString(stream, 104)),
      _timestamp(Stream::getLong64(stream, 96)),
      DBfilePath(db_file)
{
    /*
    размеры двоичных данных записываемых в файл
    |block_size|id     |status | pass_h  |  pass_s  |  time  | login_size  |  login|name_size|name|
               4       8       12        32         96      104           108
    */

    auto pos = stream.tellg();
    uint block_size = Stream::getUint(stream);
    _status = (user::status)Stream::getUint(stream, 8);
    _name = Stream::getString(stream, Stream::getUint(stream, 104) + 108);
    _pass_hash = new uint[5];
    uint pass_pos = 12;
    for (int i{0}; i < 5; i++)
    {
        _pass_hash[i] = Stream::getUint(stream, pass_pos);
        pass_pos += 4;
    }
    stream.seekg(pos += 32);
    stream.read(_pass_salt, 64);
    stream.seekg(pos += (block_size - 32) + 4);
}

User::~User()
{
    delete[] _pass_hash;
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

    std::cout << "Рег." << StampToTime(_timestamp) << std::endl;
    std::cout << std::endl;
}

void User::setPass(std::string &pass)
{
    setSalt(abs(pass[0]));
    char message[_pass_bytes];
    bytesForHash(pass, message);
    _pass_hash = sha1(message, _pass_bytes);
    pass = std::string();
    clearHashMessage(message);
}
bool User::validatePass(std::string &pass)
{
    char message[_pass_bytes];
    bytesForHash(pass, message);
    uint *pass_hash = sha1(message, _pass_bytes);
    pass = std::string();
    clearHashMessage(message);
    for (int i = 0; i < 5; i++)
    {
        if (pass_hash[i] != _pass_hash[i])
        {
            return false;
        }
    }
    return true;
}

void User::writeData()
{
    /*
    размеры двоичных данных записываемых в файл
    |block_size|id     |status | pass_h  |  pass_s  |  time  | login_size  |  login|name_size|name|
               4       8       12        32         96      104           108
    */
    std::ofstream stream(DBfilePath, std::ios::app | std::ios::ate | std::ios::binary);
    stream << "USER";
    const uint uintSize = sizeof(uint);
    const uint loginSize = _login.size();
    const uint nameSize = _name.size();
    const uint pashSize = uintSize * 5;
    const uint passSaltSize = 64;
    const uint status = (int)_status;
    const uint longSize = sizeof(unsigned long long);

    const uint block_size = 108 - 4 + loginSize + 4 + nameSize;

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

    // pass
    for (int i{0}; i < 5; i++)
    {
        memcpy(uint_num, &_pass_hash[i], uintSize);
        stream.write(uint_num, uintSize);
    }
    // salt
    stream.write(_pass_salt, passSaltSize);

    // timestamp
    memcpy(long_num, &_timestamp, longSize);
    stream.write(long_num, longSize);

    // login
    memcpy(uint_num, &loginSize, uintSize);
    stream.write(uint_num, uintSize); // login len
    stream << _login;                 // login data

    // name
    memcpy(uint_num, &nameSize, uintSize);
    stream.write(uint_num, uintSize);
    stream << _name;

    stream.close();
    std::filesystem::permissions(
        DBfilePath,
        std::filesystem::perms::owner_all & ~(std::filesystem::perms::group_all |
                                              std::filesystem::perms::others_all));
}

void User::bytesForHash(const std::string &pass, char message[])
{
    int half_length = _pass_bytes / 2;
    for (int i{0}; i < half_length; i++)
    {
        char symbol = i < pass.length() ? abs(pass[i]) : '0';
        message[i] = symbol;
    }
    for (int i{half_length}; i < _pass_bytes; i++)
    {
        message[i] = _pass_salt[i - half_length];
    }
}
void User::setSalt(const unsigned &seed)
{
    std::srand(time(NULL) - seed);
    for (int i{0}; i < 50; i++)
    {
        _pass_salt[i] = std::abs(std::rand()) % 126;
    }
}
void User::clearHashMessage(char message[])
{
    for (int i{_pass_bytes}; i < _pass_bytes; i++)
    {
        message[i] = 0;
    }
}
