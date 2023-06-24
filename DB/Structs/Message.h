#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Flags.h"
#include "../Options.h"
#include "../../Misc/Misc.h"
#include <filesystem>
#include <fstream>
#include <cstring>
#include "../../Misc/Stream.h"

typedef unsigned int uint;

class Message
{
private:
    const uint _id;
    const unsigned long long _timestamp;
    const uint _author_id;
    const uint _recipient_id;
    const std::string _text;
    msg::status _status;
    Flags<msg::status> flags;

    std::string DBfilePath;

public:
    ~Message() = default;

    /// @brief Создает сообщение чата путем прямого указания данных
    /// @param id
    /// @param author_id
    /// @param recipient_id
    /// @param text
    /// @param _status
    /// @param db_file
    Message(
        const uint &id,
        const uint &author_id,
        const uint &recipient_id,
        const std::string &text,
        msg::status _status,
        const std::string &db_file);

    /// @brief Создает сообщение из файла
    /// @param stream
    /// @param db_file
    Message(std::ifstream &stream, const std::string &db_file);
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
    std::string getText();

    /// @brief Выводит на экран данные сообщения.
    void printData();

    std::string getData();

    /// @brief Записывает в файл данные сообщения
    void writeData();

    uint getOwnerID();
};