#pragma once

#include <string>
#include <iostream>
#include <limits>
#include "Logger.h"

class Configuration final
{
public:
    static const std::string PRODUCT_DATA_FILE; 
    static const std::string USER_DATA_FILE;
    static Logger* s_logger;

    //Статичні утиліти
    static void ClearInputBuffer();

    template <typename T>
    static T GetSafeNumberInput(const std::string& prompt);

private:
    Configuration() = delete;
};

template <typename T>
T Configuration::GetSafeNumberInput(const std::string& prompt)
{
    T value;
    std::cout << prompt;
    
    while (!(std::cin >> value) || value < 0) 
    {
        std::cerr << "// Помилка: Некоректний ввід. Будь ласка, введіть додатне число: ";
        ClearInputBuffer();
    }
    ClearInputBuffer(); 
    
    return value;
}