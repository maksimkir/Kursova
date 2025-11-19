#include "StoreManager.h"
#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iomanip>

StoreManager::StoreManager(const std::string& productFile) : m_productFile(productFile)
{
    LoadProducts();
}

StoreManager::~StoreManager()
{
    SaveProducts();
}

bool StoreManager::LoadProducts()
{
    std::ifstream file(m_productFile);
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line); // Пропускаємо заголовок

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, unit, lastSupplyDate, sPrice, sQuantity;

        try {
            if (!std::getline(ss, name, ',') ||
                !std::getline(ss, unit, ',') ||
                !std::getline(ss, sPrice, ',') ||
                !std::getline(ss, sQuantity, ',') ||
                !std::getline(ss, lastSupplyDate, '\r'))
            {
                std::cerr << "// Помилка: Неповний рядок даних у файлі." << std::endl;
                continue;
            }

            double price = std::stod(sPrice);
            int quantity = std::stoi(sQuantity);

            if (price < 0 || quantity < 0) throw std::out_of_range("Negative value");

            m_products.push_back(std::make_unique<StoreItem>(name, unit, price, quantity, lastSupplyDate));

        } catch (const std::exception& e) {
            std::cerr << "// Помилка: Некоректні введення або формат даних у рядку: " << line << " (" << e.what() << ")" << std::endl;
        }
    }
    return true;
}

bool StoreManager::SaveProducts() const
{
    std::ofstream file(m_productFile);
    if (!file.is_open()) return false;

    file << "Name,Unit,Price,Quantity,LastSupplyDate" << std::endl;

    for (const auto& product : m_products) {
        file << product->ToCSVString() << std::endl;
    }
    return true;
}

StoreItem* StoreManager::FindStoreItemByName(const std::string& name)
{
    auto it = std::find_if(m_products.begin(), m_products.end(),
        [&name](const auto& p) { return p->GetName() == name; });

    if (it != m_products.end()) {
        return dynamic_cast<StoreItem*>(it->get());
    }
    return nullptr;
}

StoreItem* StoreManager::GetItemForSale(const std::string& name)
{
    return FindStoreItemByName(name);
}

void StoreManager::AddObject()
{
    std::string name, unit, lastSupplyDate;
    double price;
    int quantity;

    std::cout << "\n=== СТВОРЕННЯ НОВОГО ТОВАРУ ===" << std::endl;

    std::cout << "Введіть назву товару: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, name);

    std::cout << "Введіть одиницю виміру (шт, кг, л): ";
    std::getline(std::cin, unit);

    price = Configuration::GetSafeNumberInput<double>("Введіть ціну одиниці: ");
    quantity = Configuration::GetSafeNumberInput<int>("Введіть кількість: ");

    std::cout << "Введіть дату останнього завозу (YYYYMMDD): ";
    std::getline(std::cin, lastSupplyDate);

    if (FindStoreItemByName(name) != nullptr) {
        std::cerr << "// Помилка: Товар з такою назвою вже існує. Операцію скасовано." << std::endl;
        return;
    }

    m_products.push_back(std::make_unique<StoreItem>(name, unit, price, quantity, lastSupplyDate));
    std::cout << "// Товар '" << name << "' успішно додано до бази." << std::endl;
}

void StoreManager::EditObject()
{
    std::string name;
    std::cout << "\n=== РЕДАГУВАННЯ ТОВАРУ ===" << std::endl;
    std::cout << "Введіть назву товару для редагування: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, name);

    StoreItem* item = FindStoreItemByName(name);

    if (item) {
        int choice;
        std::cout << "Що редагувати? 1. Ціна 2. Кількість 3. Одиниця виміру: ";
        if (!(std::cin >> choice)) {
            std::cerr << "// Невірний ввід." << std::endl;
            Configuration::ClearInputBuffer();
            return;
        }

        switch (choice) {
            case 1: {
                double newPrice = Configuration::GetSafeNumberInput<double>("Введіть нову ціну: ");
                item->SetPrice(newPrice);
                break;
            }
            case 2: {
                int newQuantity = Configuration::GetSafeNumberInput<int>("Введіть нову кількість: ");
                item->SetQuantity(newQuantity);
                break;
            }
            case 3: {
                std::string newUnit;
                std::cout << "Введіть нову одиницю виміру: ";
                Configuration::ClearInputBuffer();
                std::getline(std::cin, newUnit);
                item->SetUnit(newUnit);
                break;
            }
            default:
                std::cerr << "// Невірний вибір поля." << std::endl;
                return;
        }
        std::cout << "// Товар '" << name << "' успішно оновлено." << std::endl;
    } else {
        std::cerr << "// Помилка: Товар не знайдено." << std::endl;
    }
}

void StoreManager::DeleteObject()
{
    std::string name;
    std::cout << "\n=== ВИДАЛЕННЯ ТОВАРУ (Списання/Уцінка) ===" << std::endl;
    std::cout << "Введіть назву товару для видалення: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, name);

    auto oldSize = m_products.size();

    m_products.erase(std::remove_if(m_products.begin(), m_products.end(),
        [&](const auto& p) { return p->GetName() == name; }), m_products.end());

    if (m_products.size() < oldSize) {
        std::cout << "// Товар '" << name << "' успішно списано/видалено з бази." << std::endl;
    } else {
        std::cerr << "// Помилка: Товар не знайдено." << std::endl;
    }
}

void StoreManager::ViewObjects() const
{
    if (m_products.empty()) {
        std::cout << "// Список товарів порожній." << std::endl;
        return;
    }

    std::cout << "\n================================== СПИСОК ТОВАРІВ ==================================" << std::endl;
    std::cout << "Назва                 | Ціна    | Залишок | Од. | Дата Завозу" << std::endl;
    std::cout << "----------------------|---------|---------|-----|------------" << std::endl;

    for (const auto& product : m_products) {
        // Використовуємо dynamic_cast для доступу до полів StoreItem (якщо потрібно)
        if (auto item = dynamic_cast<StoreItem*>(product.get())) {
            std::cout << std::left << std::setw(22) << item->GetName() << "|"
                      << std::right << std::setw(7) << std::fixed << std::setprecision(2) << item->GetPrice() << " |"
                      << std::right << std::setw(7) << item->GetQuantity() << " |"
                      << std::left << std::setw(3) << item->GetUnit() << " | "
                      << item->GetLastSupplyDate() << std::endl;
        }
    }
    std::cout << "===================================================================================" << std::endl;
}

void StoreManager::SearchObjects() const
{
    std::string criteria;
    std::cout << "Введіть частину назви для пошуку: ";
    Configuration::ClearInputBuffer();
    std::getline(std::cin, criteria);

    if (criteria.empty()) {
        std::cerr << "// Помилка: Критерій пошуку не може бути порожнім." << std::endl;
        return;
    }

    std::cout << "\n======== РЕЗУЛЬТАТИ ПОШУКУ за '" << criteria << "' ========" << std::endl;
    bool found = false;
    for (const auto& product : m_products) {
        if (product->GetName().find(criteria) != std::string::npos) {
            product->DisplayInfo();
            found = true;
        }
    }
    if (!found) {
        std::cout << "// Товарів, що відповідають критерію, не знайдено." << std::endl;
    }
}

void StoreManager::InternalSort(std::function<bool(const ProductPointer&, const ProductPointer&)> comparator)
{
    std::sort(m_products.begin(), m_products.end(), comparator);
}

void StoreManager::SortObjects()
{
    int choice;
    std::cout << "Сортувати за: 1. Назва (ASC) 2. Ціна (ASC) 3. Кількість (DESC): ";
    if (!(std::cin >> choice)) {
        std::cerr << "// Невірний ввід." << std::endl;
        Configuration::ClearInputBuffer();
        return;
    }
    Configuration::ClearInputBuffer();

    switch (choice) {
        case 1:
            InternalSort([](const auto& a, const auto& b) { return a->GetName() < b->GetName(); });
            break;
        case 2:
            InternalSort([](const auto& a, const auto& b) { return a->GetPrice() < b->GetPrice(); });
            break;
        case 3:
            InternalSort([](const auto& a, const auto& b) {
                return dynamic_cast<StoreItem*>(a.get())->GetQuantity() >
                       dynamic_cast<StoreItem*>(b.get())->GetQuantity();
            });
            break;
        default:
            std::cerr << "Невірний вибір." << std::endl;
            return;
    }
    std::cout << "// Сортування виконано." << std::endl;
}

void StoreManager::FilterObjects() const
{
    int minQuantity;
    std::cout << "Показати товари, у яких залишок менший за: ";
    minQuantity = Configuration::GetSafeNumberInput<int>("");

    std::cout << "\n======== ФІЛЬТР: ЗАЛИШОК < " << minQuantity << " ========" << std::endl;
    bool found = false;
    for (const auto& product : m_products) {
        if (auto item = dynamic_cast<StoreItem*>(product.get())) {
            if (item->GetQuantity() < minQuantity) {
                item->DisplayInfo();
                found = true;
            }
        }
    }
    if (!found) {
        std::cout << "// Товарів, що відповідають критерію, не знайдено." << std::endl;
    }
}

void StoreManager::PerformInventory() const
{
    double totalValue = 0.0;

    std::cout << "\n============================ ІНВЕНТАРИЗАЦІЯ ЗАЛИШКІВ ============================" << std::endl;
    std::cout << std::left << std::setw(25) << "Товар" << "|" << std::right << std::setw(10) << "Залишок"
              << "|" << std::setw(10) << "Ціна" << "|" << std::setw(15) << "Вартість" << std::endl;
    std::cout << "-------------------------|----------|----------|---------------" << std::endl;

    for (const auto& product : m_products) {
        if (auto item = dynamic_cast<StoreItem*>(product.get())) {
            double productValue = item->GetPrice() * item->GetQuantity();
            totalValue += productValue;

            std::cout << std::left << std::setw(25) << item->GetName() << "|"
                      << std::right << std::setw(10) << item->GetQuantity() << "|"
                      << std::setw(10) << std::fixed << std::setprecision(2) << item->GetPrice() << "|"
                      << std::setw(15) << productValue << std::endl;
        }
    }
    std::cout << "-----------------------------------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(47) << "СУМАРНА ВАРТІСТЬ ЗАЛИШКІВ:" << std::right << std::setw(17)
              << std::fixed << std::setprecision(2) << totalValue << " грн." << std::endl;
}

void StoreManager::DisplayHelp() const
{
    std::cout << "\n============================== ІНСТРУКЦІЯ КОРИСТУВАЧА (HELP) ==============================" << std::endl;

    std::cout << "1. ПОЯСНЕННЯ РОБОТИ ПРОГРАМИ:" << std::endl;
    std::cout << "   - Програма є консольним додатком для управління базою товарів магазину." << std::endl;
    std::cout << "   - Робота починається з авторизації (Admin має повні права, User - обмежені)." << std::endl;
    std::cout << "   - Дані про товари зберігаються у файлі 'store_items.csv', дані користувачів - 'users.txt'." << std::endl;
    std::cout << "   - При виході дані автоматично зберігаються (4.2. Зберігає оновлені дані)." << std::endl;

    std::cout << "\n2. ПРАВИЛА ВВОДУ ДАНИХ:" << std::endl;
    std::cout << "   - Усі числові поля (ціна, кількість) вимагають введення додатних чисел." << std::endl;
    std::cout << "   - Система автоматично перевіряє коректність формату та діапазону введених даних (3.1. Перевірка коректності)." << std::endl;

    std::cout << "\n3. КОРОТКИЙ ОПИС КОМАНД (Касир):" << std::endl;
    std::cout << "   - [1] Оформити покупку: Відкриває цикл продажу, виписує чек та коректує базу." << std::endl;
    std::cout << "   - [2] Переглянути: Виводить повний список товарів (1.5)." << std::endl;
    std::cout << "   - [3] Шукати: Дозволяє знайти товар за частиною назви (1.5)." << std::endl;
    std::cout << "   - [0] Вийти: Вихід з облікового запису." << std::endl;

    std::cout << "===================================================================================" << std::endl;
}