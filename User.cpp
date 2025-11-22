#include "User.h"


User::User(const std::string& login, const std::string& password)
    : m_login(login), m_password(password), m_isAuthenticated(false) 
{
}
