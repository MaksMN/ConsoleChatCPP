#pragma once
#include "DBcore.h"

/// @brief Класс управления жалобами
class DBcomplaints : public DBcore<Complaint>
{
private:
public:
    ~DBcomplaints() = default;

    std::shared_ptr<Complaint> addComplaint(

        const uint &troubleMsgID,
        const uint &applicantUID,
        const uint &defendantUID,
        complaint::status status,
        const std::string &complaintText,
        const std::string &messageText);

    std::shared_ptr<Complaint> addComplaint(
        const uint &&troubleMsgID,
        const uint &&applicantUID,
        const uint &&defendantUID,
        complaint::status status,
        const std::string &&complaintText,
        const std::string &&messageText);

    /// @brief Ищет в базе жалобу по ID
    /// @param id
    /// @return
    std::shared_ptr<Complaint> getComplaintByID(uint id);
};
