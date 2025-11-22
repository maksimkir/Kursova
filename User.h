#pragma once

#include <string>
#include <iostream>

class User
{
public:
    User(const std::string& login, const std::string& password);

    virtual ~User() = default;

    //Поліморфні методи (чисто віртуальні)
    virtual bool IsAdmin() const = 0; 
    virtual void DisplayMenu() const = 0; 

    std::string GetLogin() const { return m_login; }
    std::string GetPassword() const { return m_password; }
    bool IsAuthenticated() const { return m_isAuthenticated; }
    void SetAuthenticated(bool status) { m_isAuthenticated = status; }

protected:
    std::string m_login;
    std::string m_password;
    bool m_isAuthenticated;
};