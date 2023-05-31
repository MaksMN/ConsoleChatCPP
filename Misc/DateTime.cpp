#include "DateTime.h"

std::wstring StampToTime(long long timestamp)
{
     time_t tick = (time_t)(timestamp);
    struct tm tm;
    char s[40];
    tm = *localtime(&tick);
    std::wstring day = (tm.tm_mday < 10) ? L"0" + std::to_wstring(tm.tm_mday) : std::to_wstring(tm.tm_mday);
    std::wstring month = (tm.tm_mon < 10) ? L"0" + std::to_wstring(tm.tm_mon) : std::to_wstring(tm.tm_mon);
    std::wstring year = std::to_wstring(tm.tm_year + 1900);
    std::wstring hour = (tm.tm_hour < 10) ? L"0" + std::to_wstring(tm.tm_hour) : std::to_wstring(tm.tm_hour);
    std::wstring min = (tm.tm_min < 10) ? L"0" + std::to_wstring(tm.tm_min) : std::to_wstring(tm.tm_min);
    std::wstring sec = (tm.tm_sec < 10) ? L"0" + std::to_wstring(tm.tm_sec) : std::to_wstring(tm.tm_sec);
    return std::wstring(day + L"." + month + L"." + year + L" " + hour + L":" + min + L":" + sec);
}

// Функция имеет такой формат потому-что в Windows преобразователи структуры tm не работают с wstring