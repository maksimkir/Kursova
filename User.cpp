#include "User.h"

// Реалізація конструктора
User::User(const std::string& login, const std::string& password)
    : m_login(login), m_password(password), m_isAuthenticated(false) 
{
    // Тіло конструктора залишається простим, оскільки це абстрактний клас.
}
// Деструктор віртуальний, але тіло не потрібне (використовуємо = default)