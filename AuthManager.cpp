#include "AuthManager.h"
#include "Configuration.h"
#include "Admin.h"
#include "BasicUser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdexcept>

AuthManager::AuthManager(const std::string& userFile) : m_userFile(userFile)
{
    Configuration::s_logger->LogInfo("Ініціалізація AuthManager.");
    LoadUsersFromFile();
}

AuthManager::~AuthManager()
{
    SaveUsers();
    Configuration::s_logger->LogInfo("AuthManager знищено. Дані користувачів збережено.");
}

bool AuthManager::LoadUsersFromFile()
{
    std::ifstream file(m_userFile);
    if (!file.is_open()) {
        Configuration::s_logger->LogInfo("Файл користувачів не знайдено. Створення базового Admin.");
        std::cout << "// Попередження: Файл користувачів не знайдено. Створення базового Admin." << std::endl;
        // Створення базового Admin, якщо файл порожній (для ініціалізації)
        m_users.push_back(std::make_unique<Admin>("admin", "admin123"));
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');

        try {
            if (colonPos != std::string::npos) {
                std::string login = line.substr(0, colonPos);
                std::string password = line.substr(colonPos + 1);

                bool isAdmin = (login == "admin");

                if (isAdmin) {
                    m_users.push_back(std::make_unique<Admin>(login, password));
                } else {
                    m_users.push_back(std::make_unique<BasicUser>(login, password));
                }
            } else {
                throw std::runtime_error("Невірний формат рядка (відсутній ':')");
            }
        } catch (const std::exception& e) {
            Configuration::s_logger->LogError("Помилка формату даних у users.txt: " + line + " (" + e.what() + ")");
            std::cerr << "// Помилка формату даних у users.txt: " << e.what() << std::endl;
        }
    }
    Configuration::s_logger->LogInfo("Користувачів успішно завантажено: " + std::to_string(m_users.size()));
    return true;
}

void AuthManager::SaveUsers() const
{
    std::ofstream file(m_userFile);
    if (!file.is_open()) {
        Configuration::s_logger->LogError("Не вдалося зберегти дані користувачів.");
        std::cerr << "// Помилка: Не вдалося зберегти дані користувачів." << std::endl;
        return;
    }

    // Паролі зберігаються у відкритому вигляді
    for (const auto& user : m_users) {
        file << user->GetLogin() << ":" << user->GetPassword() << std::endl;
    }
}

bool AuthManager::ValidateUserCredentials(const std::string& login, const std::string& password) const
{
    // 3. Обробка помилок та надійність: перевірка коректності даних
    if (login.empty() || password.empty()) {
        Configuration::s_logger->LogError("Спроба авторизації з порожніми логіном/паролем.");
        std::cerr << "// Помилка: Логін або пароль не можуть бути порожніми." << std::endl;
        return false;
    }
    return true;
}

bool AuthManager::AuthenticateUser(const std::string& login, const std::string& password)
{
    if (!ValidateUserCredentials(login, password)) return false;

    auto it = std::find_if(m_users.begin(), m_users.end(),
        [&](const auto& user) {
            return user->GetLogin() == login && user->GetPassword() == password;
        });

    if (it != m_users.end()) {
        // Доступ до основних функцій надається лише після успішної авторизації
        if ((*it)->IsAdmin()) {
            m_currentUser = std::make_unique<Admin>(login, password);
        } else {
            m_currentUser = std::make_unique<BasicUser>(login, password);
        }
        m_currentUser->SetAuthenticated(true);
        Configuration::s_logger->LogInfo("Успішна авторизація користувача: " + login + (m_currentUser->IsAdmin() ? " (Admin)" : " (User)")); // Журналювання
        std::cout << "// Авторизація успішна. Вітаємо, " << login << "!" << std::endl;
        return true;
    }

    Configuration::s_logger->LogError("Невірна спроба авторизації для логіна: " + login);
    std::cerr << "// Помилка: Невірний логін або пароль." << std::endl;
    return false;
}

void AuthManager::AddNewUser()
{
    std::string login, password, roleStr;
    std::cout << "--- СТВОРЕННЯ КОРИСТУВАЧА ---" << std::endl;
    std::cout << "Введіть новий логін: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, login);

    if (std::any_of(m_users.begin(), m_users.end(), [&](const auto& u){ return u->GetLogin() == login; })) {
        Configuration::s_logger->LogError("Спроба додати існуючого користувача: " + login); // Журналювання
        std::cerr << "// Помилка: Користувач з логіном '" << login << "' вже існує." << std::endl;
        return;
    }

    std::cout << "Введіть пароль: ";
    std::getline(std::cin, password);

    std::cout << "Введіть роль (admin/user): ";
    std::getline(std::cin, roleStr);
    std::transform(roleStr.begin(), roleStr.end(), roleStr.begin(), ::tolower);

    bool isAdmin = (roleStr == "admin");

    if (isAdmin) {
        m_users.push_back(std::make_unique<Admin>(login, password));
    } else {
        m_users.push_back(std::make_unique<BasicUser>(login, password));
    }

    Configuration::s_logger->LogInfo("Створено нового користувача: " + login + (isAdmin ? " (Admin)" : " (User)")); // Журналювання
    std::cout << "// Користувач '" << login << "' (" << (isAdmin ? "Admin" : "User") << ") успішно доданий." << std::endl;
}

void AuthManager::DeleteUser()
{
    std::string login;
    std::cout << "--- ВИДАЛЕННЯ КОРИСТУВАЧА ---" << std::endl;
    std::cout << "Введіть логін для видалення: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, login);

    if (m_currentUser && m_currentUser->GetLogin() == login) {
        Configuration::s_logger->LogError("Спроба видалити власний обліковий запис: " + login); // Журналювання
        std::cerr << "// Помилка: Не можна видалити власний обліковий запис." << std::endl;
        return;
    }

    auto oldSize = m_users.size();

    m_users.erase(std::remove_if(m_users.begin(), m_users.end(),
        [&](const auto& u) { return u->GetLogin() == login; }), m_users.end());

    if (m_users.size() < oldSize) {
        Configuration::s_logger->LogInfo("Користувача видалено: " + login); // Журналювання
        std::cout << "// Користувач '" << login << "' успішно видалений." << std::endl;
    } else {
        Configuration::s_logger->LogError("Спроба видалити неіснуючого користувача: " + login); // Журналювання
        std::cerr << "// Помилка: Користувач '" << login << "' не знайдений." << std::endl;
    }
}

void AuthManager::DisplayAllUsers() const
{
    std::cout << "\n================= ЗАРЕЄСТРОВАНІ КОРИСТУВАЧІ ==================" << std::endl;
    std::cout << std::left << std::setw(20) << "ЛОГІН" << "|"
              << std::left << std::setw(15) << "ПАРОЛЬ (ВІДКРИТО)" << "|"
              << "РОЛЬ" << std::endl;
    std::cout << "--------------------|---------------|-----" << std::endl;

    for (const auto& user : m_users) {
        std::cout << std::left << std::setw(20) << user->GetLogin() << "|"
                  << std::left << std::setw(15) << user->GetPassword() << "|"
                  << (user->IsAdmin() ? "Admin" : "User") << std::endl;
    }
    std::cout << "==============================================================" << std::endl;
}

void AuthManager::AdminUsersMenu()
{
    int choice;
    do {
        std::cout << "\n=== МЕНЮ КЕРУВАННЯ КОРИСТУВАЧАМИ ===" << std::endl;
        std::cout << "1. Переглянути всіх користувачів" << std::endl;
        std::cout << "2. Створити нового користувача" << std::endl;
        std::cout << "3. Видалити користувача" << std::endl;
        std::cout << "0. Повернутися до головного меню" << std::endl;
        std::cout << "Ваш вибір: ";

        if (!(std::cin >> choice)) {
            Configuration::s_logger->LogError("Некоректний ввід у меню керування користувачами."); // Журналювання
            std::cerr << "// Некоректний ввід." << std::endl;
            Configuration::ClearInputBuffer();
            continue;
        }
        Configuration::ClearInputBuffer();

        switch (choice) {
            case 1: DisplayAllUsers(); break;
            case 2: AddNewUser(); break;
            case 3: DeleteUser(); break;
            case 0: break;
            default:
                Configuration::s_logger->LogError("Невірний вибір AdminUsersMenu: " + std::to_string(choice)); // Журналювання
                std::cerr << "// Невірний вибір." << std::endl;
                break;
        }
    } while (choice != 0);
}