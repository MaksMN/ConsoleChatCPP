#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <time.h>

/// @brief Выводит сообщение в консоль. Кроссплатформенная.
/// @param wmesg Текст сообщения
/// @param endl true - добавить перевод строки
void printMessage(std::string wmesg, bool endl = true);

/// @brief Преобразует timestamp в обычное время
/// @param timestamp
/// @return
std::string StampToTime(long long timestamp);