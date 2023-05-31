#include "Complaint.h"

Complaint::Complaint(const uint id,
                     const uint troubleMsgID,
                     const uint applicantUID,
                     const uint defendantUID,
                     complaint::status status,
                     const std::wstring &complaintText,
                     const std::wstring &messageText)
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

std::wstring Complaint::getComplaintText()
{
    return _complaintText;
}

std::wstring Complaint::getMessageText()
{
    return _messageText;
}

void Complaint::printData()
{
    std::wcout << L"[complaint ID " << _id << L"] Дата подачи жалобы: " << StampToTime(_timestamp) << L" ";
    if (isOpen())
    {
        std::wcout << L" [status open]";
    }
    if (isComplete())
    {
        std::wcout << L" [status complete]";
    }
    std::wcout << std::endl;
    std::wcout << _complaintText << std::endl;
    std::wcout << std::endl;
}
