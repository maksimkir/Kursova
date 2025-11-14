#include "Receipt.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>

Receipt::Receipt(int receiptId, const std::string& cashierName)
    : m_receiptId(receiptId), m_cashierName(cashierName)
{
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    m_timestamp = ss.str();
}

void Receipt::AddItem(const std::string& itemName, double price, int quantity)
{
    m_items.push_back({itemName, price, quantity});
}

double Receipt::CalculateTotal() const
{
    double total = 0.0;
    for (const auto& item : m_items) {
        total += item.lineTotal();
    }
    return total;
}

void Receipt::DisplayInfo() const
{
    std::cout << "\n==================================================" << std::endl;
    std::cout << std::left << std::setw(40) << "МАГАЗИН: СИСТЕМА УПРАВЛІННЯ" << std::endl;
    std::cout << "Чек #" << std::left << std::setw(30) << m_receiptId 
              << " | Дата: " << m_timestamp << std::endl;
    std::cout << "Касир: " << m_cashierName << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(20) << "Товар" << std::right << std::setw(8) << "Ціна" 
              << std::setw(5) << "К-ть" << std::setw(12) << "СУМА" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (const auto& item : m_items) {
        std::cout << std::left << std::setw(20) << item.name 
                  << std::right << std::setw(8) << std::fixed << std::setprecision(2) << item.price 
                  << std::setw(5) << item.quantity 
                  << std::setw(12) << item.lineTotal() << std::endl;
    }
    
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(35) << "ВСЬОГО ДО СПЛАТИ:" << std::right << std::setw(14) 
              << std::fixed << std::setprecision(2) << CalculateTotal() << " грн" << std::endl;
    std::cout << "==================================================" << std::endl;
}