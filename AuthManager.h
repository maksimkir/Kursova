#pragma once

#include "User.h" // Тепер це лише абстрактний клас User
#include <vector>
#include <memory>
#include <string>

/**
 * @brief Клас AuthManager. Керує колекцією користувачів та їхньою авторизацією.
 */
class AuthManager final
{
public:
    AuthManager(const std::string& userFile);
    ~AuthManager();

    // 1.6. Функції авторизації та виходу
    bool AuthenticateUser(const std::string& login, const std::string& password);
    void Logout() { m_currentUser.reset(); }

    // 1.6. Функції адміністратора (Керування користувачами)
    void AddNewUser();
    void DeleteUser();
    void DisplayAllUsers() const;
    void AdminUsersMenu();

    User* GetCurrentUser() const { return m_currentUser.get(); }

private:
    std::vector<std::unique_ptr<User>> m_users; // 2.4. Використання std::vector та std::unique_ptr
    std::unique_ptr<User> m_currentUser;
    const std::string m_userFile;

    // 4. Робота з файлами
    bool LoadUsersFromFile(); // 4.1. Програма зчитує дані з файлів при запуску.
    void SaveUsers() const; // 4.2. При виході — зберігає оновлені дані.
    
    // 3. Обробка помилок та надійність
    bool ValidateUserCredentials(const std::string& login, const std::string& password) const;
};