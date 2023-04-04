//
// Message.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once
#include <string>
#define private      \
    friend class DB; \
private

struct Message
{

    // true - a message from a private chat
    // false = true - a message from a public chat
    bool isPrivate{false};

    /// @brief default init
    Message() = default;

    /// @brief default deinit
    ~Message() = default;

    /// @brief init
    /// @param message string message
    /// @param authorID int author ID
    /// @param recipientID int recipient ID
    /// @param privateChat bool -> true - private chat / false - public chat
    Message(const std::wstring &message, int authorID, int recipientID, bool privateChat = false) : isPrivate(privateChat),
                                                                                                  _message(message),
                                                                                                  _authorID(authorID),
                                                                                                  _recipientID(recipientID) {}

    /// @brief Assigns a message to an object
    /// @param message string
    void setMessage(const std::wstring &message);

    /// @brief Assigns a author ID to an object
    /// @param authorID int -> ID of the author of the message
    void setAuthorID(int authorID);

    /// @brief Assigns a recipient ID to an object
    /// @param recipientID int -> ID of the message recipient
    void setRecipientID(int recipientID);

    /// @brief Returns a message as a string
    /// @return string
    const std::wstring &getMessage() const;

    /// @brief Returns the ID of the author of the message
    /// @return int author ID
    int getAuthorID() const;

    /// @brief Returns the ID of the recipient of the message
    /// @return int recipient ID
    int getRecipientID() const;

    /// @brief Returns the ID of the message
    /// @return int
    long long int getId() const;

    /// @brief Unix Timestamp returns the date in seconds from 1970
    /// @return long long int -> timestamp date in seconds
    long long int getDate() const;

#undef private
    // Private methods for Friends L"DB"

    /// @brief set date Unix Timestamp
    void setDateMessage();

    /// @brief  set message unique id
    void setMessageId();

    /// @brief Overloading the assignment operator L"="
    /// @param message Message to assign struct L"Message"
    /// @return Message
    Message &operator=(const Message &message);

private:
    std::wstring _message;
    int _authorID = 0;
    int _recipientID = 0;
    long long int _date{0};
    int _id{0};
    static long long int _currentId;
};
