#pragma once

#include <string>
#include <iostream>

/**
 * @brief Абстрактний базовий клас User. Забезпечує базову інкапсуляцію
 * та поліморфізм для прав доступу.
 */
class User 
{

public:
    User(const std::string& login, const std::string& password);

    virtual ~User() = default;

    // == Поліморфні методи (чисто віртуальні) ==
    virtual bool IsAdmin() const = 0; 
    virtual void DisplayMenu() const = 0; 

    // == Get/Set методи ==
    std::string GetLogin() const { return m_login; }
    std::string GetPassword() const { return m_password; }
    bool IsAuthenticated() const { return m_isAuthenticated; }
    void SetAuthenticated(bool status) { m_isAuthenticated = status; }

protected:
    std::string m_login;
    std::string m_password;
    bool m_isAuthenticated;
};