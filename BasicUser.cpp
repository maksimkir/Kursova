#include "BasicUser.h"
#include <iostream>

void BasicUser::DisplayMenu() const
{
    std::cout << "\n======== Меню Касира (КОРИСТУВАЧ) =========" << std::endl;
    std::cout << "1. Оформити покупку (Продаж)" << std::endl;
    std::cout << "2. Переглянути всі товари" << std::endl;
    std::cout << "3. Шукати товари" << std::endl;
    std::cout << "4. Вивести інструкцію (Help)" << std::endl;
    std::cout << "0. Вийти з облікового запису" << std::endl;
}