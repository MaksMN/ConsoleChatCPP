#pragma once
#include <string>
#include <iostream>
#include "../../Misc/Flags.h"
#include "../Options.h"
#include "../../Misc/DateTime.h"
typedef unsigned int uint;

/// @brief Класс описывающий структуру жалобы
class Complaint
{
private:
    const uint _id;
    const unsigned long long _timestamp;
    const uint _troubleMsgID;
    const uint _applicantUID;
    const uint _defendantUID;
    complaint::status _status;
    const std::string _complaintText;
    const std::string _messageText;

    Flags<complaint::status> flags;

public:
    Complaint(
        const uint id,
        const uint troubleMsgID,
        const uint applicantUID,
        const uint defendantUID,
        complaint::status status,
        const std::string &complaintText,
        const std::string &messageText);
    ~Complaint() = default;

    /// @brief Возвращает ID жалобы
    /// @return
    uint getID();

    /// @brief Возвращает ID сообщения на которе составлена эта жалоба
    /// @return
    uint getTroubleMsgID();

    /// @brief Возвращает ID пользователя, написавшего жалобу
    /// @return
    uint getApplicantUID();

    /// @brief Возвращает ID пользователя на которого пожаловались
    /// @return
    uint getDefendantUID();

    /// @brief Возвращает статус жалобы
    /// @return
    complaint::status getStatus();

    /// @brief Проверяет открыта ли жалоба
    /// @return
    bool isOpen();

    /// @brief Проверяет закрыта ли жалоба
    /// @return
    bool isComplete();

    /// @brief Открывает жалобу
    void open();

    /// @brief Помечает жалобу решенной
    void complete();

    /// @brief Возвращает текст жалобы
    /// @return
    std::string getComplaintText();

    /// @brief Возвращает текст сообщения на которе эта жалоба
    /// @return
    std::string getMessageText();

    /// @brief Выводит на экран данные жалобы
    void printData();
};