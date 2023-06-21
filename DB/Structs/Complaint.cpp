#include "Complaint.h"

Complaint::Complaint(const uint id,
                     const uint troubleMsgID,
                     const uint applicantUID,
                     const uint defendantUID,
                     complaint::status status,
                     const std::string &complaintText,
                     const std::string &messageText)
    : _id(id),
      _troubleMsgID(troubleMsgID),
      _applicantUID(applicantUID),
      _defendantUID(defendantUID),
      _status(status),
      _complaintText(complaintText),
      _messageText(messageText),
      _timestamp(time(NULL)) {}

uint Complaint::getID()
{
    return _id;
}

uint Complaint::getTroubleMsgID()
{
    return _troubleMsgID;
}

uint Complaint::getApplicantUID()
{
    return _applicantUID;
}

uint Complaint::getDefendantUID()
{
    return _defendantUID;
}

complaint::status Complaint::getStatus()
{
    return _status;
}

bool Complaint::isOpen()
{
    return flags.hasFlag(_status, complaint::open_);
}

bool Complaint::isComplete()
{
    return flags.hasFlag(_status, complaint::complete_);
}

void Complaint::open()
{
    _status = flags.flagsReplace(_status, complaint::open_, complaint::complete_);
}

void Complaint::complete()
{
    _status = flags.flagsReplace(_status, complaint::complete_, complaint::open_);
}

std::string Complaint::getComplaintText()
{
    return _complaintText;
}

std::string Complaint::getMessageText()
{
    return _messageText;
}

void Complaint::printData()
{
    std::cout << "[complaint ID " << _id << "] Дата подачи жалобы: " << Misc::StampToTime(_timestamp) << " ";
    if (isOpen())
    {
        std::cout << " [status open]";
    }
    if (isComplete())
    {
        std::cout << " [status complete]";
    }
    std::cout << std::endl;
    std::cout << _complaintText << std::endl;
    std::cout << std::endl;
}

std::string Complaint::getData()
{
    std::string s1 = "[complaint ID " + std::to_string(_id) + "] Дата подачи жалобы: " + Misc::StampToTime(_timestamp) + " ";
    if (isOpen())
    {
        s1 += " [status open]";
    }
    if (isComplete())
    {
        s1 += " [status complete]";
    }

    s1 += "\n" + _complaintText + "\n";
    return s1;
}

uint Complaint::getOwnerID()
{
    return _applicantUID;
}
