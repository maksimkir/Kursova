#pragma once

#include <string>
#include <vector>
#include "IPrintable.h"

/**
 * @brief Клас Receipt. Модель даних для виписаного чека (Оформлення покупки: виписка чека).
 */
class Receipt final : public IPrintable
{
public:
    Receipt(int receiptId, const std::string& cashierName);

    struct ReceiptItem {
        std::string name;
        double price;
        int quantity;
        double lineTotal() const { return price * quantity; }
    };
    
    void AddItem(const std::string& itemName, double price, int quantity);
    double CalculateTotal() const;
    int GetReceiptId() const { return m_receiptId; }
    void DisplayInfo() const override; // 5.1. Виводить чек у форматованому вигляді (IPrintable)

private:
    int m_receiptId;
    std::string m_cashierName;
    std::string m_timestamp;
    std::vector<ReceiptItem> m_items;
};