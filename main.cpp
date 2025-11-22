#include <complex>

#include "AuthManager.h"
#include "StoreManager.h"
#include "Configuration.h"
#include "SaleProcessor.h"
#include <iostream>
#include <limits>
#include <stdexcept>

int main()
{
    try {
        //ініціалізація основних менеджерів
        AuthManager authManager(Configuration::USER_DATA_FILE);
        StoreManager storeManager(Configuration::PRODUCT_DATA_FILE);
        SaleProcessor saleProcessor(storeManager);

        User* currentUser = nullptr;
        std::string login, password;
        int choice = -1;

        while (true)
        {
            try {
                if (!currentUser) {
                    std::cout << "\n======== СИСТЕМА УПРАВЛІННЯ МАГАЗИНОМ ========" << std::endl;
                    std::cout << "Введіть 0 для виходу або дані для авторизації." << std::endl;
                    std::cout << "Логін: ";
                    std::cin >> login;
                    if (login == "0") break;
                    std::cout << "Пароль: ";
                    std::cin >> password;
                    Configuration::ClearInputBuffer();

                    if (authManager.AuthenticateUser(login, password)) {
                        currentUser = authManager.GetCurrentUser();
                    } else {
                        continue;
                    }
                }

                currentUser->DisplayMenu();
                std::cout << "Ваш вибір: ";

                if (!(std::cin >> choice)) {
                    std::cerr << "// Некоректний ввід. Повторіть." << std::endl;
                    Configuration::ClearInputBuffer();
                    continue;
                }
                Configuration::ClearInputBuffer();

                if (currentUser->IsAdmin()) {
                    //обробка меню Адміністратора
                    try {
                        switch (choice) {
                            case 1: storeManager.AddObject(); break;
                            case 2: storeManager.EditObject(); break;
                            case 3: storeManager.DeleteObject(); break;
                            case 4: storeManager.ViewObjects(); break;
                            case 5: storeManager.SearchObjects(); break;
                            case 6: storeManager.SortObjects(); break;
                            case 7: storeManager.FilterObjects(); break;
                            case 8: storeManager.PerformInventory(); break;
                            case 9: authManager.AdminUsersMenu(); break;
                            case 0: authManager.Logout(); currentUser = nullptr; break;
                            default: std::cerr << "// Невірний вибір." << std::endl; break;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "// Помилка при виконанні операції адміністратора: " << e.what() << std::endl;
                        Configuration::s_logger->LogError("Помилка в меню адміністратора: " + std::string(e.what()));
                    }
                } else {
                    try {
                        switch (choice) {
                            case 1: saleProcessor.ProcessFullSale(currentUser->GetLogin()); break;
                            case 2: storeManager.ViewObjects(); break;
                            case 3: storeManager.SearchObjects(); break;
                            case 4: storeManager.DisplayHelp(); break;
                            case 0: authManager.Logout(); currentUser = nullptr; break;
                            default: std::cerr << "// Невірний вибір." << std::endl; break;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "// Помилка при виконанні операції: " << e.what() << std::endl;
                        Configuration::s_logger->LogError("Помилка в меню користувача: " + std::string(e.what()));
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "// Критична помилка в циклі програми: " << e.what() << std::endl;
                Configuration::s_logger->LogError("Критична помилка в main loop: " + std::string(e.what()));
                Configuration::ClearInputBuffer();
            }
        }

        std::cout << "Програма завершила роботу. Дані збережено." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "// КРИТИЧНА ПОМИЛКА: " << e.what() << std::endl;
        std::cerr << "Програма завершує роботу через невиправну помилку." << std::endl;
        Configuration::s_logger->LogError("Критична помилка програми: " + std::string(e.what()));
        return 1;
    } catch (...) {
        std::cerr << "// НЕВІДОМА КРИТИЧНА ПОМИЛКА" << std::endl;
        std::cerr << "Програма завершує роботу через невиправну помилку." << std::endl;
        Configuration::s_logger->LogError("Невідома критична помилка програми");
        return 1;
    }
    
    return 0;
}