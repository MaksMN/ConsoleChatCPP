#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <cstring>
#include <locale>
#include <time.h>
#include <vector>

typedef unsigned int uint;
namespace Misc
{

    /// @brief Выводит сообщение в консоль. Кроссплатформенная.
    /// @param wmesg Текст сообщения
    /// @param endl true - добавить перевод строки
    void printMessage(std::string wmesg, bool endl = true);

    /// @brief Преобразует timestamp в обычное время
    /// @param timestamp
    /// @return
    std::string StampToTime(long long timestamp);

    /**
     * Функции чтения буфера
     */

    /// @brief Читает int число из указанной позиции буфера
    /// @param buffer
    /// @param offset
    /// @return
    uint getInt(char buffer[], uint offset = 0);

    /// @brief Читает unsigned long long число из указанной позиции буфера
    /// @param buffer
    /// @param offset
    /// @return
    unsigned long long getLong(char buffer[], uint offset = 0);

    /// @brief Читает строку из указанной позиции буфера. Чтение начинается с метки длинны строки.
    /// @param buffer
    /// @param offset
    /// @return
    std::string getString(char buffer[], uint offset = 0);

    /// @brief Записывает int в буфер
    /// @param num
    /// @param buffer
    /// @param offset
    void writeIntBuffer(uint num, char buffer[], uint offset = 0);
    /// @brief Записывает long в буфер
    /// @param num
    /// @param buffer
    /// @param offset
    void writeUlongBuffer(unsigned long long num, char buffer[], uint offset = 0);

    /// @brief Записывает строку в буфер. Перед блоком данных пишет размер блока
    /// @param str
    /// @param buffer
    /// @param offset
    /// @param add_size true - добавить размер блока в начале
    void writeStringBuffer(std::string str, char buffer[], uint offset = 0, bool add_size = true);

    /// @brief Записывает буфер в вектор
    /// @param buffer
    /// @param len
    /// @return
    std::vector<char> writeVectorBuffer(char buffer[], uint len);
    /// @brief Записывает вектор в буфер
    /// @param buffer
    /// @param vector
    void writeVectorBuffer(char buffer[], std::vector<char> &vector);
    /// @brief Записывает сроку в вектор
    /// @param str
    /// @return
    std::vector<char> writeVectorBuffer(std::string str);

} // namespace name