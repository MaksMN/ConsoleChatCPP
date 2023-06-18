#pragma once
#include "Flags.h"
#include "Misc.h"

#define FLAGS_ADDR 0
#define DYN_DATA_PTR_ADDR 1
#define SESSION_KEY_ADDR 2
#define PG_MODE_ADDR 10
#define PG_START_ADDR 11
#define PG_PERPAGE_ADDR 15
#define PG_END_ADDR 19

#define DYN_DATA_ADDR 23
#define LOGIN_COUNT 0
#define PAGE_TEXT_COUNT 1
#define CMD_TEXT_COUNT 2

#define CMD_BUFFER 1024 // Размер буфера команд и заголовков

typedef unsigned int uint;
typedef unsigned long long ullong;

/*

    Командный пакет
    |флаги(1)|DYN DATA(1)|session_key (8)|pg_mode(1)|pg_start(4)|pg_perPage(4)|pg_end(4)|login_size(4)|login|page_size(4)|PAGE_TEXT|cmd_size(4)|CMD_TEXT|
    0        1           2               10         11          15            19        23
    |___________________________________________________________________________________|
    |                                  ^                                                |
    |                               static                                              |  dynamic

    1 байт указывает где находятся динамические данные
*/

namespace sv
{
    enum options
    {
        // в одном байте максимальный флаг 128
        get_string = 1,    // Сообщает клиенту: надо ввести строку
        get_int = 2,       // Сообщает клиенту: надо ввести число
        write_session = 4, // Сообщает клиенту: записать в буфер сессионный ключ, пришедший с сервера.
        clear_console = 8  // Сообщает клиенту: очистить окно консоли
    };

    enum pagination
    {
        message = 1,  // отобразить сообщение по его номеру в списке всех сообщений
        last_page = 2 // показать последние 10 сообщений
    };
};

/// @brief Класс работы с командным буфером
class BufferActions
{
private:
    sv::options flags = (sv::options)0;
    Flags<sv::options> f;
    uint flags_count = 0;
    uint dyndata_count = 0;
    char (&cmd_buffer)[CMD_BUFFER];

public:
    BufferActions(char (&_cmd_buffer)[CMD_BUFFER]);
    ~BufferActions() = default;

    void createFlags() {}
    template <typename T, typename... Args>
    void createFlags(T value, Args... args);

    void addFlags() {}
    template <typename T, typename... Args>
    void addFlags(T value, Args... args);

    /// @brief Удаляет флаг
    void removeFlag(sv::options option);

    /// @brief Проверяет наличие флага в сумме
    /// @param option
    /// @return
    bool hasFlag(sv::options option);

    void writeDynData() {}
    template <typename T, typename... Args>
    void writeDynData(T value, Args... args);

    /// @brief Записывает в буфер динамические данные в заданную позицию от первого блока
    /// @param data
    /// @param blockCount
    void writeDynDataPos(std::string data, uint blockCount);

    /// @brief Записывает в динамические данные буфера число в заданную позицию от первого блока
    /// @param value
    /// @param blockCount
    void writeDynDataPos(uint value, uint blockCount);

    /// @brief получает адрес начала блока динамических данных
    /// @return
    char getDynDataAddr();

    ullong getSessionKey();
    void setSessionKey(ullong key);

    sv::pagination getPaginationMode();
    void setPaginationMode(sv::pagination mode);

    uint getPgStart();
    void setPgStart(uint value);

    uint getPgPerPage();
    void setPgPerPage(uint value);

    uint getPgEnd();
    void setPgEnd(uint value);

    uint getDynDataSize(uint blockCount);

    std::string getDynData(uint blockCount);
};

/// @brief Добавляет в буфер флаги, очищает буфер флагов
/// @tparam T
/// @tparam ...Args
/// @param value
/// @param ...args
template <typename T, typename... Args>
inline void BufferActions::createFlags(T value, Args... args)
{
    if (flags_count == 0)
        flags = (sv::options)0;
    flags_count++;

    flags = f.addFlag(flags, value);

    if (value == sv::get_string)
        flags = f.removeFlag(flags, sv::get_int);

    if (value == sv::get_int)
        flags = f.removeFlag(flags, sv::get_string);

    createFlags(args...);
    flags_count = 0;
    cmd_buffer[FLAGS_ADDR] = (char)flags;
}

/// @brief Добавляет в буфер флаги, не очищает буфер флагов
/// @tparam T
/// @tparam ...Args
/// @param value
/// @param ...args
template <typename T, typename... Args>
inline void BufferActions::addFlags(T value, Args... args)
{
    flags = f.addFlag(flags, value);

    if (value == sv::get_string)
        flags = f.removeFlag(flags, sv::get_int);

    if (value == sv::get_int)
        flags = f.removeFlag(flags, sv::get_string);

    addFlags(args...);
    cmd_buffer[FLAGS_ADDR] = (char)flags;
}

/// @brief Записывает в буфер динамические данные в порядке их следования от первого блока
/// @tparam T
/// @tparam ...Args
/// @param value
/// @param ...args
template <typename T, typename... Args>
inline void BufferActions::writeDynData(T value, Args... args)
{
    if (dyndata_count == 0)
    {
        Misc::writeStringBuffer(value, cmd_buffer, DYN_DATA_ADDR);
    }
    if (dyndata_count > 0)
    {
        Misc::writeStringBuffer(value, cmd_buffer, Misc::findDynamicData(cmd_buffer, DYN_DATA_ADDR, dyndata_count));
    }
    dyndata_count++;
    writeDynData(args...);
    dyndata_count = 0;
}
