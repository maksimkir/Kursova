#include "Admin.h"
#include <iostream>

void Admin::DisplayMenu() const
{
    std::cout << "\n=== Меню Адміністратора (АДМІН) ===" << std::endl;
    std::cout << "1. Додати новий товар" << std::endl;
    std::cout << "2. Редагувати товар" << std::endl;
    std::cout << "3. Видалити товар (Уцінка/Списання)" << std::endl;
    std::cout << "4. Переглянути всі товари" << std::endl;
    std::cout << "5. Шукати товари" << std::endl;
    std::cout << "6. Сортувати товари" << std::endl;
    std::cout << "7. Фільтрувати товари" << std::endl;
    std::cout << "8. Провести інвентаризацію залишків" << std::endl;
    std::cout << "--- Адміністрування користувачів ---" << std::endl;
    std::cout << "9. Керування обліковими записами" << std::endl;
    std::cout << "0. Вийти з облікового запису" << std::endl;
}