#pragma once

#include <string>
#include <iostream>

/**
 * @brief Простий клас для журналювання подій.
 */
class Logger
{
public:
    void LogInfo(const std::string& message) const
    {
        std::cout << "[INFO] " << message << std::endl;
    }

    void LogError(const std::string& message) const
    {
        std::cerr << "[ERROR] " << message << std::endl;
    }
};

