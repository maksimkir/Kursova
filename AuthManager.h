#pragma once

#include "User.h"
#include <vector>
#include <memory>
#include <string>


class AuthManager final
{
public:
    AuthManager(const std::string& userFile);
    ~AuthManager();

    bool AuthenticateUser(const std::string& login, const std::string& password);
    void Logout() { m_currentUser.reset(); }

    void AddNewUser();
    void DeleteUser();
    void DisplayAllUsers() const;
    void AdminUsersMenu();

    User* GetCurrentUser() const { return m_currentUser.get(); }

private:
    std::vector<std::unique_ptr<User>> m_users;
    std::unique_ptr<User> m_currentUser;
    const std::string m_userFile;

    bool LoadUsersFromFile();
    void SaveUsers() const;


    bool ValidateUserCredentials(const std::string& login, const std::string& password) const;
};