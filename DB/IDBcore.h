#pragma once
#include "Options.h"
#include "Structs/User.h"
#include "Structs/Message.h"
#include "Structs/Complaint.h"
#include "../Misc/Flags.h"
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>

/// @brief Базовый класс для работы с БД чата
/// @tparam T
template <typename T>
class IDBcore
{
protected:
    std::vector<std::shared_ptr<T>> _DB; // основная база всех данных

public:
    virtual ~IDBcore() = default;

    /// @brief Возвращает диапазон элементов из базы.
    /// @param start Индекс первого элемента
    /// @param perPage Элементов на странице

    std::vector<std::shared_ptr<T>> list(uint &start, const uint &perPage, uint &end);

    /// @brief возвращает последние perPage элементов из базы.
    /// @param perPage Элементов на странице
    std::vector<std::shared_ptr<T>> listLast(uint &start, const uint &perPage, uint &end);

    /// @brief Возвращает количество элементов в базе
    /// @return
    uint getCount();

    /// @brief Проверяет не пустая ли база
    /// @return true если пустая
    bool empty();

    /// @brief Извлекает элемент базы по ID элемента. Не по индексу вектора.
    /// @param id
    /// @return
    std::shared_ptr<T> operator[](uint id);

    /// @brief Удаляет элемент из базы по ID элемента. Не по индексу вектора.
    /// @param id
    void remove(uint id);

    void readFromFile(std::string path, std::string marker, uint &last);
    void updateFiles(std::string path);
};

template <typename T>
inline std::vector<std::shared_ptr<T>> IDBcore<T>::list(uint &start, const uint &perPage, uint &end)
{
    if (_DB.empty())
    {
        return std::vector<std::shared_ptr<T>>();
    }
    std::vector<std::shared_ptr<T>> v;
    end = start + perPage;
    if (start > _DB.size())
        start = 0;
    if (end > _DB.size())
        end = _DB.size();

    for (uint i{start}; i < end; i++)
    {
        if (i == _DB.size())
            break;
        v.push_back(_DB[i]);
    }
    return v;
}

template <typename T>
inline std::vector<std::shared_ptr<T>> IDBcore<T>::listLast(uint &start, const uint &perPage, uint &end)
{
    if (_DB.empty())
    {
        return std::vector<std::shared_ptr<T>>();
    }

    end = _DB.size();
    if (perPage >= _DB.size())
    {
        start = 0;
    }
    else
    {
        start = _DB.size() - perPage;
    }
    return list(start, perPage, end);
}

template <typename T>
inline uint IDBcore<T>::getCount()
{
    return _DB.size();
}

template <typename T>
inline bool IDBcore<T>::empty()
{
    return _DB.empty();
}

template <typename T>
inline std::shared_ptr<T> IDBcore<T>::operator[](uint id)
{
    auto it = std::find_if(_DB.begin(), _DB.end(),
                           [&id](const auto &t)
                           {
                               return t->getID() == id;
                           });

    if (_DB.end() != it)
        return *it;
    return nullptr;
}

template <typename T>
inline void IDBcore<T>::remove(uint id)
{
    auto it = std::find_if(_DB.begin(), _DB.end(),
                           [&id](const auto &t)
                           {
                               return t->getID() == id;
                           });

    if (_DB.end() != it)
        _DB.erase(it);
}

template <typename T>
inline void IDBcore<T>::readFromFile(std::string path, std::string marker, uint &last)
{
    if(!std::filesystem::exists(path)){
        return;
    }
    std::ifstream stream(path, std::ios::app | std::ios::ate | std::ios::binary);
    stream.seekg(0);

    while (!stream.eof())
    {
        char u[4];
        stream.read(u, 4);
        auto pos = stream.tellg();
        std::string u_(u, 4);
        if (u_ != marker || pos == -1)
            break;

        _DB.push_back(std::make_shared<T>(stream, path));
    }
    stream.close();
    last = _DB.back()->getID();
}

template <typename T>
inline void IDBcore<T>::updateFiles(std::string path)
{
    std::filesystem::remove(path);
    for (int i{0}; i < _DB.size(); i++)
    {
        _DB[i]->writeData();
    }
}
