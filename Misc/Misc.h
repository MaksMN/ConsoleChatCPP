#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <cstring>
#include <locale>
#include <time.h>
#include <vector>

typedef unsigned int uint;
typedef unsigned long long ullong;
namespace Misc
{

    /// @brief Выводит сообщение в консоль. Кроссплатформенная.
    /// @param wmesg Текст сообщения
    /// @param endl true - добавить перевод строки
    void printMessage(const std::string &msg, bool endl = true);

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

    /// @brief Читает строку из буфера. Чтение начинается с offset на длину len.
    /// @param buffer
    /// @param len
    /// @param offset
    /// @return
    std::string getString(const char buffer[], uint len, uint offset);

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
    /// @param max_size усечет строку спереди если она больше этого размера
    void writeStringBuffer(std::string &str, char buffer[], uint offset = 0, bool add_size = true, uint max_size = 4092);
    void writeStringBuffer(std::string &&str, char buffer[], uint offset = 0, bool add_size = true, uint max_size = 4092);

    /// @brief Ищет позицию блока динамических данных в буфере
    /// @param buffer
    /// @param offset Адрес блока динамических данных
    /// @param offset_data Номер блока динамических данных
    /// @return
    uint findDynamicData(char buffer[], uint offset = 0, uint offset_data = 0, uint max_size = 1024);

    /// @brief Записывает буфер в вектор
    /// @param buffer
    /// @param len
    /// @return
    std::vector<char> writeVectorBuffer(char buffer[], uint len);
    /// @brief Записывает вектор в буфер
    /// @param buffer
    /// @param vector
    void writeVectorBuffer(char buffer[], const std::vector<char> &vector);
    /// @brief Записывает сроку в вектор
    /// @param str
    /// @return
    std::vector<char> writeVectorBuffer(const std::string &str);

    /// @brief Генерирует случайное число 8 байт
    /// @return
    ullong getRandomKey();

    /// @brief Разбивает строку делителем.
    /// @param str
    /// @param delimeter
    /// @return Если в строке нет делителя или он по краям строки, возвращает исходную строку
    std::vector<std::string> stringExplode(std::string const &str, const std::string &delimeter);
    std::vector<std::string> stringExplode(std::string const &str, char delimeter);

    std::string ltrimString(std::string &string, uint max);

}