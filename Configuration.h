#pragma once

#include <string>
#include <iostream>
#include <limits>
#include "Logger.h"

/**
 * @brief Клас Configuration. Зберігає глобальні константи та загальні утиліти.
 */
class Configuration final
{
public:
    // == Статичні константи ==
    static const std::string PRODUCT_DATA_FILE; 
    static const std::string USER_DATA_FILE;
    static Logger* s_logger;

    // == Статичні утиліти для покращення інтерфейсу та надійності (3.1, 3.2) ==
    static void ClearInputBuffer();

    /**
     * @brief Забезпечує безпечний ввід числового значення.
     * Реалізує необхідну обробку виключень через try-catch та перевірку некоректних введень.
     */
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