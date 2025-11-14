#pragma once

#include "User.h" // Успадковуємо від базового класу User

/**
 * @brief Клас Admin. Має повні права.
 */
class Admin final : public User
{
public:
    Admin(const std::string& login, const std::string& password)
        : User(login, password) {}

    bool IsAdmin() const override { return true; }

    void DisplayMenu() const override; // Оголошення у .h
};