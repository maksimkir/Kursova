#include <complex>

#include "AuthManager.h"
#include "StoreManager.h"
#include "Configuration.h"
#include "SaleProcessor.h"
#include <iostream>
#include <limits>

int main()
{
    //ініціалізація основних менеджерів
    AuthManager authManager(Configuration::USER_DATA_FILE);
    StoreManager storeManager(Configuration::PRODUCT_DATA_FILE);
    SaleProcessor saleProcessor(storeManager);

    User* currentUser = nullptr;
    std::string login, password;
    int choice = -1;

    while (true)
    {
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
        } else {
            //обробка меню Користувача
            switch (choice) {
                case 1: saleProcessor.ProcessFullSale(currentUser->GetLogin()); break;
                case 2: storeManager.ViewObjects(); break;
                case 3: storeManager.SearchObjects(); break;
                case 4: storeManager.DisplayHelp(); break;std:
                case 0: authManager.Logout(); currentUser = nullptr; break;

                default: std::cerr << "// Невірний вибір." << std::endl; break;
            }
        }
    }

    std::cout << "Програма завершила роботу. Дані збережено." << std::endl;
    return 0;
}