#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Flags.h"
#include "../Options.h"
#include "../../Misc/DateTime.h"
typedef unsigned int uint;

class Message
{
private:
    const uint _id;
    const unsigned long long _timestamp;
    const uint _author_id;
    const uint _recipient_id;
    const std::wstring _text;
    msg::status _status;
    Flags<msg::status> flags;

public:
    ~Message() = default;
    Message(
        const uint &id,
        const uint &author_id,
        const uint &recipient_id,
        const std::wstring &text,
        msg::status _status);

    /// @brief Возвращает статус сообщения
    /// @return
    msg::status getStatus();

    /// @brief Изменяет статус сообщения
    /// @param status
    void setStatus(msg::status status);

    /// @brief Добавляет флаг скрытия
    void hide();

    /// @brief Проверяет есть ли в сообщении флаг скрытия
    /// @return
    bool isHidden();

    /// @brief Удаляет флаг скрытия
    void show();

    /// @brief Добавляет флаг публичного сообщения, удаляет приватного
    void to_public();

    /// @brief Добавляет флаг приватного и удаляет флаг публичного сообщения
    void to_private();

    /// @brief Возвращает ID сообщения
    /// @return
    uint getID();

    /// @brief Возвращает ID автора сообщения
    /// @return
    uint getAuthorID();

    /// @brief Возвращает ID получателя приватного сообщения
    /// @return
    uint getRecipientID();

    /// @brief Возвращает текст сообщения
    /// @return
    std::wstring getText();

    /// @brief Выводит на экран данные сообщения.
    void printData();
};