//
// DB.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once

#include "DBCore.h"
#include "User.h"
#include "Message.h"

class DB
{
    DBCore<User> _userDB;       // database User
    DBCore<Message> _messageDB; // database Message

    /// @brief private method : Adds public messages to newly registered users
    /// @param id int user id
    /// @return
    int addMessagesToNewUserFromPublicChat(int id);

    /// @brief private method :
    /// @param arrayId Array of numbers
    /// @param id int The number to check
    /// @param size Array size
    /// @return true If the L"id" is present in the array
    bool isUsedId(std::unique_ptr<int[]> const &arrayId, int id, int size) const;

public:
    /// @brief default init
    DB() = default;

    /// @brief default deinit
    ~DB() = default;

    /// @brief adds a new user to the database
    /// @param name string
    /// @param login string
    /// @param pass string -> unencoded password
    /// @return true - if is success, false if login non-unique!
    bool addUser(const std::wstring &name,
                 const std::wstring &login,
                 const std::wstring &pass);

    /// @brief adds a new user to the database
    /// @param user struct User
    /// @return true - if is success, false if login non-unique!
    bool addUser(User &user);

    /// @brief adds a message to the database in a private chat
    /// @param message struct Message type
    void addMessage(Message &message);

    /// @brief adds a message to the database in a public chat, for all users
    /// @param message
    void AddMessageToAllUsers(Message &message);

    /// @brief Sets the user account password
    /// @param userID int unique id
    /// @param pass std::wstring new password
    /// @return true - if is success, false - if there is no user with this id
    bool setUserPassword(int userID, const std::wstring &pass);

    /// @brief Checks whether the login is unique.
    /// @param login std::wstring.
    /// @return true - if login unique, false - if the login exists.
    bool isUniqueLogin(const std::wstring &login);

    /// @brief checks whether passwords match or not
    /// @param userID int ID user
    /// @param pass std::wstring incoming password
    /// @return true - if the passwords match, false - if the passwords are different
    bool isCorrectPassword(int userID, const std::wstring &pass);

    /// @brief Returns an array of users
    /// @return  if the array is not empty - std::unique_ptr<User[]> , nullptr if the array is empty!
    const std::unique_ptr<User[]> getAllUsers() const;

    /// @brief Returns an array of users found by name
    /// @param name User name
    /// @param size the reference is the size of the array, the created size of the array will be written to it
    /// @return if the array is not empty - std::unique_ptr<User[]> , nullptr if the array is empty!
    const std::unique_ptr<User[]> getAllUsers(const std::wstring &name, int &size) const;

    /// @brief Get a user by login
    /// @param login string -> user login
    /// @param exception
    /// @return Returning a pointer to a user object
    const std::unique_ptr<User> getUserByLogin(const std::wstring &login, bool exception = false) const;

    /// @brief Get a user by ID
    /// @param userId
    /// @return
    const std::unique_ptr<User> getUserById(const int &userId) const;

    /// @brief Returns the size of the user array
    /// @return  int user array size
    int usersCount() const;

    /// @brief Returns an array of all messages
    /// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
    const std::unique_ptr<Message[]> getAllMessages() const;

    const std::unique_ptr<Message> getMessage(const int &messageId) const;

    /// @brief Returns an array of all incoming/outgoing messages for - user with ID, in chronological ascending order
    /// @param id int unique recipientID
    /// @return if the array is not empty - std::unique_ptr<Message[]>, nullptr if the array is empty!
    const std::unique_ptr<Message[]> getAllMessagesForUserById(int id) const;

    /// @brief Updates the user's data (name, login), except for the password!
    /// @param user struct User
    /// @return true if successful, false if not a unique login, no user, no users
    bool updateUserData(const User &user);

    /// @brief Deletes the user's account, marked as deleted. The password is reset, the name is changed to the L"deleted"
    /// @param user struct User
    /// @return true if successful, false if not a unique login, no user, no users
    bool deleteUserAccount(User &user); // true -> if account deleted, false -> failed

    /// @brief
    /// @param message
    /// @return
    bool deleteMessageById(int Id);

    /// @brief Returns an array of all incoming/outgoing Private messages between users (user1 and user2) in chronological ascending order
    /// @param user1Id int ID of the first user
    /// @param user2Id int ID of the second user
    /// @param size the reference is the size of the array, the created size of the array will be written to it
    /// @return std::unique_ptr<Message[]> Returns an array
    const std::unique_ptr<Message[]> getAllPrivateMessagesForUsersById(int user1Id, int user2Id, int &size) const;

    /// @brief Returns an array of all incoming/outgoing Private messages between user in chronological ascending order
    /// @param userId
    /// @param size the reference is the size of the array, the created size of the array will be written to it
    /// @return std::unique_ptr<Message[]> Returns an array
    const std::unique_ptr<Message[]> getAllPrivateMessagesForUserById(int userId, int &size) const;

    /// @brief Returns an array of all incoming/outgoing Public messages between users with ID in chronological ascending order
    /// @param Id int ID of the user
    /// @param size the reference is the size of the array, the created size of the array will be written to it
    /// @return std::unique_ptr<Message[]> Returns an array
    const std::unique_ptr<Message[]> getAllPublicMessagesForUserById(int Id, int &size) const;

    /// @brief Returns an array of all incoming/outgoing Public chronological ascending order
    /// @param size the reference is the size of the array, the created size of the array will be written to it
    /// @return std::unique_ptr<Message[]> Returns an array
    const std::unique_ptr<Message[]> getAllPublicMessages(int &size) const;
};