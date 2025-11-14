#include "StoreItem.h"
#include <sstream>
#include <iostream>
#include <iomanip>

StoreItem::StoreItem() 
    : m_name(""), m_unit(""), m_price(0.0), m_quantity(0), m_lastSupplyDate("") {}

StoreItem::StoreItem(const std::string& name, const std::string& unit, double price, int quantity, const std::string& lastSupplyDate)
    : m_name(name), m_unit(unit), m_price(price), m_quantity(quantity), m_lastSupplyDate(lastSupplyDate) {}

StoreItem::StoreItem(const StoreItem& other)
    : Product(other), m_name(other.m_name), m_unit(other.m_unit), m_price(other.m_price), m_quantity(other.m_quantity), m_lastSupplyDate(other.m_lastSupplyDate) {}

StoreItem::~StoreItem() {}

void StoreItem::DisplayInfo() const
{
    std::cout << "--- Товар: " << std::left << std::setw(20) << m_name 
              << " | Ціна: " << std::fixed << std::setprecision(2) << m_price 
              << " | Залишок: " << m_quantity 
              << " " << m_unit << " | Завезено: " << m_lastSupplyDate << std::endl;
}

std::string StoreItem::ToCSVString() const
{
    std::stringstream ss;
    ss << m_name << "," << m_unit << "," << m_price << "," << m_quantity << "," << m_lastSupplyDate;
    return ss.str();
}