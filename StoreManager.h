#pragma once

#include "StoreItem.h"
#include "Configuration.h"
#include <vector>
#include <memory>
#include <functional>

using ProductPointer = std::unique_ptr<Product>;
using ProductCollection = std::vector<ProductPointer>;

class StoreManager final
{
public:
    StoreManager(const std::string& productFile);
    ~StoreManager();

    void AddObject(); // Додавати нові об'єкти (Реєстрація надходження товару)
    void EditObject(); // Редагувати існуючі
    void DeleteObject(); // Видаляти об'єкти (Проблема уцінки і списання)
    void ViewObjects() const; // Переглядати списки об'єктів

    void SearchObjects() const; // Шукати за критеріями
    void SortObjects(); // Сортувати дані
    void FilterObjects() const; // Фільтрувати записи за заданою умовою
    void PerformInventory() const; // Інвентаризація залишків товару з обчисленням сумарної вартості
    void DisplayHelp() const; // Виводити інструкцію користувача

    // Метод для роботи з продажами
    StoreItem* GetItemForSale(const std::string& name);

private:
    ProductCollection m_products;
    const std::string m_productFile;


    bool LoadProducts(); //Програма зчитує дані з файлів при запуску.
    bool SaveProducts() const; //При виході — зберігає оновлені дані.


    StoreItem* FindStoreItemByName(const std::string& name);
    void InternalSort(std::function<bool(const ProductPointer&, const ProductPointer&)> comparator);
};