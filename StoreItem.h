#pragma once

#include "Product.h"

/**
 * @brief Конкретна реалізація товару магазину (StoreItem).
 * База наявності товарів: найменування, одиниця виміру, ціна одиниці, кількість, дата останнього завозу.
 */
class StoreItem final : public Product
{
public:
    // Конструктори та Деструктор
    StoreItem();
    StoreItem(const std::string& name, const std::string& unit, double price, int quantity, const std::string& lastSupplyDate);
    StoreItem(const StoreItem& other); // Копіювальний конструктор
    ~StoreItem(); 

    // == Get/Set методи (PascalCase) ==
    std::string GetName() const override { return m_name; }
    void SetName(const std::string& name) override { m_name = name; }
    std::string GetUnit() const { return m_unit; }
    void SetUnit(const std::string& unit) { m_unit = unit; }
    double GetPrice() const override { return m_price; }
    void SetPrice(double price) override { m_price = price; }
    int GetQuantity() const { return m_quantity; }
    void SetQuantity(int quantity) { m_quantity = quantity; }
    void UpdateQuantity(int change) { m_quantity += change; } 
    std::string GetLastSupplyDate() const { return m_lastSupplyDate; }
    void SetLastSupplyDate(const std::string& date) { m_lastSupplyDate = date; }

    // == Реалізація віртуальних методів ==
    void DisplayInfo() const override;
    std::string ToCSVString() const override;

private:
    // Поля (camelCase)
    std::string m_name;
    std::string m_unit;
    double m_price;
    int m_quantity;
    std::string m_lastSupplyDate;
};