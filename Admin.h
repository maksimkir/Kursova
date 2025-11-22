#pragma once

#include "User.h"

class Admin final : public User
{
public:
    Admin(const std::string& login, const std::string& password)
        : User(login, password) {}

    bool IsAdmin() const override { return true; }

    void DisplayMenu() const override;
};