#include "DBcomplaints.h"

std::shared_ptr<Complaint> DBcomplaints::addComplaint(
    const uint &troubleMsgID,
    const uint &applicantUID,
    const uint &defendantUID,
    complaint::status status,
    const std::string &complaintText,
    const std::string &messageText)
{
    _DB.push_back(std::make_shared<Complaint>(
        lastComplaintID++,
        troubleMsgID,
        applicantUID,
        defendantUID,
        status,
        complaintText,
        messageText));
    return _DB.back();
}

std::shared_ptr<Complaint> DBcomplaints::addComplaint(
    const uint &&troubleMsgID,
    const uint &&applicantUID, const uint &&defendantUID,
    complaint::status status,
    const std::string &&complaintText,
    const std::string &&messageText)
{
    return addComplaint(troubleMsgID,
                        applicantUID,
                        defendantUID,
                        status,
                        complaintText,
                        messageText);
}

std::shared_ptr<Complaint> DBcomplaints::getComplaintByID(uint id)
{
    return this->operator[](id);
}
