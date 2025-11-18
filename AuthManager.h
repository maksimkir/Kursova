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

    //авторизації та вихід
    bool AuthenticateUser(const std::string& login, const std::string& password);
    void Logout() { m_currentUser.reset(); }

    //адміністратор (Керування користувачами)
    void AddNewUser();
    void DeleteUser();
    void DisplayAllUsers() const;
    void AdminUsersMenu();

    User* GetCurrentUser() const { return m_currentUser.get(); }

private:
    std::vector<std::unique_ptr<User>> m_users;
    std::unique_ptr<User> m_currentUser;
    const std::string m_userFile;

    //.
    // 4. Робота з файлами
    bool LoadUsersFromFile(); // 4.1. Програма зчитує дані з файлів при запуску.
    void SaveUsers() const; // 4.2. При виході — зберігає оновлені дані.
    //.

    //обробка помилок
    bool ValidateUserCredentials(const std::string& login, const std::string& password) const;
};