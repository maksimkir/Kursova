#pragma once

#include <iostream>

/**
 * @brief Абстрактний інтерфейс (чисто віртуальний клас) для забезпечення
 * поліморфного виводу інформації у консоль.
 */
class IPrintable
{

public:
    /**
     * @brief Чиста віртуальна функція для виводу інформації об'єкта.
     */
    virtual void DisplayInfo() const = 0;

    /**
     * @brief Віртуальний деструктор.
     */
    virtual ~IPrintable() = default;
};