#pragma once

#include "StoreItem.h"
#include "Configuration.h"
#include <vector>
#include <memory>
#include <functional>

using ProductPointer = std::unique_ptr<Product>;
using ProductCollection = std::vector<ProductPointer>; // 2.4. Дозволяється використання std::vector

/**
 * @brief Головний контролер системи. Керує колекцією товарів (StoreItem).
 */
class StoreManager final
{
public:
    StoreManager(const std::string& productFile);
    ~StoreManager();

    // 1.5. CRUD Операції
    void AddObject(); // Додавати нові об'єкти (Реєстрація надходження товару)
    void EditObject(); // Редагувати існуючі
    void DeleteObject(); // Видаляти об'єкти (Проблема уцінки і списання)
    void ViewObjects() const; // Переглядати списки об'єктів

    // 1.5. Розширені Операції
    void SearchObjects() const; // Шукати за критеріями
    void SortObjects(); // Сортувати дані
    void FilterObjects() const; // Фільтрувати записи за заданою умовою
    void PerformInventory() const; // Інвентаризація залишків товару з обчисленням сумарної вартості
    void DisplayHelp() const; // Виводити інструкцію користувача

    // Метод для роботи з продажами (використовується SaleProcessor)
    StoreItem* GetItemForSale(const std::string& name);

private:
    ProductCollection m_products;
    const std::string m_productFile;

    // 4. Робота з файлами (4.3. Формат CSV)
    bool LoadProducts(); // 4.1. Програма зчитує дані з файлів при запуску.
    bool SaveProducts() const; // 4.2. При виході — зберігає оновлені дані.

    // Допоміжні функції
    StoreItem* FindStoreItemByName(const std::string& name);
    void InternalSort(std::function<bool(const ProductPointer&, const ProductPointer&)> comparator);
};