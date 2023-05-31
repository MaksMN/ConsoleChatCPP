#pragma once

/// @brief Опции и флаги статусов
namespace msg
{
    enum status
    {
        hidden_ = 1,   // сообщение скрыто
        public_ = 2,   // публичное сообщение
        private_ = 4,  // личное сообщение
        complain_ = 8, // жалоба
        complain_complete
    };
};

namespace complaint
{
    enum status
    {
        open_ = 1,
        complete_ = 2
    };
};

namespace user
{
    enum status
    {
        user_ = 1,
        admin_ = 2,
        bot_ = 4,
        banned_ = 8,
        none_ = 16
    };

    enum options
    {
        set_pass,
        set_login,
        set_name,
        complete_
    };
};