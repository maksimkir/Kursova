#pragma once

#include "User.h"


class BasicUser final : public User
{
public:
    BasicUser(const std::string& login, const std::string& password)
        : User(login, password) {}

    bool IsAdmin() const override { return false; }

    void DisplayMenu() const override;
};