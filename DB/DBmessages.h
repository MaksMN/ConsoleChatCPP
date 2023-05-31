#pragma once
#include "IDBcore.h"

class DBmessages : public IDBcore<Message>
{
private:
    uint lastMsgID = 0;

public:
    ~DBmessages() = default;

    std::shared_ptr<Message> addMessage(const uint &author_id,
                                        const uint &recipient_id,
                                        const std::wstring &text,
                                        msg::status status);
    /// @brief Ищет в базе сообщение по ID
    /// @param id
    /// @return
    std::shared_ptr<Message> getMessageByID(uint id);

    /// @brief Выводит список личных сообщений. Параметры пагинации вычисляются автоматически и возвращаются по ссылке
    /// @param author_id ID Автора
    /// @param recipient_id ID Получателя
    /// @param start Параметр пагинации
    /// @param perPage Параметр пагинации
    /// @param end Параметр пагинации
    /// @param last (bool) true - вычислят список последних perPage сообщений. false - perPage от указанного сообщения.
    /// @return
    std::vector<std::shared_ptr<Message>> getPrivateMsgList(uint &&author_id, uint &&recipient_id, uint &start, const uint &perPage, uint &end, bool last = true);

    /// @brief Удаляет сообщение из базы
    /// @param id
    void deleteMessage(uint id);
};