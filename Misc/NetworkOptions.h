#pragma once
#include "Flags.h"

/// @brief специфические функции для работы с флагами в буфере клиента и сервера
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
}
class NetworkOptions
{
private:
    sv::options flags = (sv::options)0;
    Flags<sv::options> f;
    int count = 0;

public:
    ~NetworkOptions() = default;
    void create() {}
    void add() {}
    template <typename T, typename... Args>
    char create(T value, Args... args);

    template <typename T, typename... Args>
    char add(T value, Args... args);

    char remove(char value, sv::options option);
    bool is(char data, sv::options option);
};

template <typename T, typename... Args>
inline char NetworkOptions::create(T value, Args... args)
{
    if (count == 0)
        flags = (sv::options)0;
    count++;

    flags = f.addFlag(flags, value);

    if (value == sv::get_string)
        flags = f.removeFlag(flags, sv::get_int);

    if (value == sv::get_int)
        flags = f.removeFlag(flags, sv::get_string);

    create(args...);
    count = 0;
    return (char)flags;
}

template <typename T, typename... Args>
inline char NetworkOptions::add(T value, Args... args)
{
    flags = f.addFlag(flags, value);

    if (value == sv::get_string)
        flags = f.removeFlag(flags, sv::get_int);

    if (value == sv::get_int)
        flags = f.removeFlag(flags, sv::get_string);

    create(args...);
    return (char)flags;
}
