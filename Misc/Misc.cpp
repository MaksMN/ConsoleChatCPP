#include "Misc.h"

void Misc::printMessage(std::string mesg, bool endl)
{
#if defined(_WIN64) || defined(_WIN32)
    std::wstring wmesg = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(mesg.data());
    std::wcout << wmesg;
    if (endl)
        std::wcout << std::endl;
#else
    std::cout << mesg;
    if (endl)
        std::cout << std::endl;
#endif
}

std::string Misc::StampToTime(long long timestamp)
{
    time_t tick = (time_t)(timestamp);
    struct tm tm;
    tm = *localtime(&tick);
    std::string day = (tm.tm_mday < 10) ? "0" + std::to_string(tm.tm_mday) : std::to_string(tm.tm_mday);
    std::string month = (tm.tm_mon < 10) ? "0" + std::to_string(tm.tm_mon) : std::to_string(tm.tm_mon);
    std::string year = std::to_string(tm.tm_year + 1900);
    std::string hour = (tm.tm_hour < 10) ? "0" + std::to_string(tm.tm_hour) : std::to_string(tm.tm_hour);
    std::string min = (tm.tm_min < 10) ? "0" + std::to_string(tm.tm_min) : std::to_string(tm.tm_min);
    std::string sec = (tm.tm_sec < 10) ? "0" + std::to_string(tm.tm_sec) : std::to_string(tm.tm_sec);
    return std::string(day + "." + month + "." + year + " " + hour + ":" + min + ":" + sec);
}

uint Misc::getInt(char buffer[], uint offset)
{
    uint out;
    memcpy(&out, &buffer[offset], 4);
    return out;
}

unsigned long long Misc::getLong(char buffer[], uint offset)
{
    unsigned long long out;
    memcpy(&out, &buffer[offset], 8);
    return out;
}

std::string Misc::getString(char buffer[], uint offset)
{
    const uint len = getInt(buffer, offset);
    char buf[len];
    offset += 4;
    for (int i{0}; i < len; i++)
        buf[i] = buffer[offset++];
    std::string s(buf, len);
    return s;
}

void Misc::writeIntBuffer(uint num, char buffer[], uint offset)
{
    mempcpy(&buffer[offset], &num, 4);
}

void Misc::writeUlongBuffer(unsigned long long num, char buffer[], uint offset)
{
    mempcpy(&buffer[offset], &num, 8);
}

void Misc::writeStringBuffer(std::string str, char buffer[], uint offset, bool add_size)
{
    const uint size = str.size();
    auto d = str.data();
    if (add_size)
    {
        mempcpy(&buffer[offset], &size, 4);
        mempcpy(&buffer[offset + 4], &d[0], size);
    }
    else
    {
        mempcpy(&buffer[offset], &d[0], size);
    }
}

std::vector<char> Misc::writeVectorBuffer(char buffer[], uint len)
{
    std::vector<char> v;
    for (int i{0}; i < len; i++)
        v.push_back(buffer[i]);
    return v;
}

void Misc::writeVectorBuffer(char buffer[], std::vector<char> &vector)
{
    const uint len = vector.size();
    for (int i{0}; i < len; i++)
        buffer[i] = vector[i];
}

std::vector<char> Misc::writeVectorBuffer(std::string str)
{
    std::vector<char> vector;
    const uint len = str.size();
    auto d = str.data();
    for (int i{0}; i < len; i++)
        vector.push_back(d[i]);

    return vector;
}

std::string Misc::cutBeginString(std::string str, const uint max_len)
{
    char buf[max_len];
    auto buf2 = str.data();
    if (str.size() > max_len)
    {

        memcpy(buf, &buf2[max_len - str.size()], max_len);
    }
    std::string s(buf, max_len);
    return s;
}