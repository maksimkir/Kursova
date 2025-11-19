#pragma once

#include <string>
#include <vector>
#include "IPrintable.h"

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
    void DisplayInfo() const override;

private:
    int m_receiptId;
    std::string m_cashierName;
    std::string m_timestamp;
    std::vector<ReceiptItem> m_items;
};