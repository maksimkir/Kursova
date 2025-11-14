#pragma once

#include "User.h" // Успадковуємо від базового класу User

/**
 * @brief Клас BasicUser. Має обмежені права (Касир).
 */
class BasicUser final : public User
{
public:
    BasicUser(const std::string& login, const std::string& password)
        : User(login, password) {}

    bool IsAdmin() const override { return false; }

    void DisplayMenu() const override; // Оголошення у .h
};