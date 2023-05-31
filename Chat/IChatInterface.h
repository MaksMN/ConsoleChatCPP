#pragma once
#include "../DB/DBmessages.h"
#include "../DB/DBusers.h"
#include "../Misc/UserInput.h"
#include "../DB/DBcomplaints.h"
#include "Options.h"
const extern char clear[];

/// @brief Базовый класс интерактивного интерфейса
class IChatInterface
{
protected:
    DBmessages &pubMessagesDB;
    DBmessages &privMessagesDB;
    DBcomplaints &complaintsDB;
    DBusers &usersDB;

    std::shared_ptr<User> AuthorizedUser = nullptr;

public:
    virtual ~IChatInterface() = default;
    IChatInterface(DBmessages &_pubMessagesDB,
                   DBmessages &_privMessagesDB,
                   DBcomplaints &_complaintsDB,
                   DBusers &_usersDB);
    virtual chat::Results run() = 0;
    chat::Results login();

    /// @brief Печатает сообщения
    /// @param messages (vector) Список пользователей
    /// @param emptyMsg Сообщение если список пустой
    /// @param beforeMsg Сообщение после вывода списка
    /// @param start С этого числа начинается нумерация сообщений. Высчитывается автоматически другими функциями работы со списками данных.
    void printMessages(std::vector<std::shared_ptr<Message>> &messages, const std::string &&emptyMsg, const std::string &&beforeMsg, const uint &start) const;

    /// @brief Выводит список элементов из БД
    /// @tparam T
    /// @param t
    /// @param emptyMsg Сообщение если список пустой
    /// @param beforeMsg Сообщение после вывода списка
    /// @param start С этого числа начинается нумерация сообщений. Высчитывается автоматически другими функциями работы со списками данных.
    template <typename T>
    void printList(std::vector<std::shared_ptr<T>> &t, const std::string &&emptyMsg, const std::string &&beforeMsg, const uint &start) const;

    /// @brief Рассчитывает пагинацию вывода на экран списков сообщений, жалоб и пользователей.
    /// @param start Первое сообщение на странице
    /// @param perPage Элементов на странице
    /// @param end Рассчитывается автоматически
    /// @param mode (chat::Pagination) последние perPage сообщений или perPage сообщений от start
    /// @param max Рассчитывается автоматически
    /// @return
    chat::Results listNavigation(uint &start, uint &perPage, uint &end, chat::Pagination &mode, uint max);
};

template <typename T>
inline void IChatInterface::printList(std::vector<std::shared_ptr<T>> &t, const std::string &&emptyMsg, const std::string &&beforeMsg, const uint &start) const
{

    if (t.empty())
    {
        std::cout << emptyMsg << std::endl;
        return;
    }
    uint k = start;
    for (auto &i : t)
    {
        std::cout << beforeMsg << "[" << ++k << "]" << std::endl;
        i->printData();
    }
}
