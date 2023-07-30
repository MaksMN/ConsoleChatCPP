#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Flags.h"
#include "ChatOptions.h"
#include "../../Misc/Misc.h"

typedef unsigned int uint;
typedef unsigned long long ullong;

class Message
{
private:
    ullong _id;
    ullong _author_id;
    ullong _recipient_id;
    std::string _text;
    ullong _published;
    msg::status _status;
    Flags<msg::status> flags;

public:
    ~Message() = default;

    /// @brief Создание нового публичного сообщения
    /// @param author_id
    /// @param text
    Message(ullong author_id, std::string text);

    /// @brief Создание публичного сообщения из базы
    /// @param author_id
    /// @param text
    /// @param published
    /// @param status
    Message(ullong id, ullong author_id, std::string text, ullong published, uint status);

    /// @brief Создание нового приватного сообщения
    /// @param author_id
    /// @param recipient_id
    /// @param text
    Message(ullong author_id, ullong recipient_id, std::string text);

    /// @brief Создание приватного сообщения из базы
    /// @param id
    /// @param author_id
    /// @param recipient_id
    /// @param text
    /// @param published
    /// @param status
    Message(ullong id, ullong author_id, ullong recipient_id, std::string text, ullong published, uint status);

    /// @brief Возвращает статус сообщения
    /// @return
    msg::status getStatus();

    uint getStatusInt();

    /// @brief Изменяет статус сообщения
    /// @param status
    void setStatus(msg::status status);

    /* добавление/удаление статусов */

    bool isPublic();
    bool isPrivate();
    void toPublic();
    void toPrivate();
    void toDelivered();
    void unDelivered();
    bool isDelivered();
    void read();
    void toUnread();
    bool isRead();

    ullong getPublished();

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
    std::string getText();

    uint getOwnerID();
};