#include "DateTime.h"

std::string StampToTime(long long timestamp)
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

// Функция имеет такой формат потому-что в Windows преобразователи структуры tm не работают с wstring