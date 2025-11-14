#pragma once

#include "Receipt.h"
#include <string>
#include <memory>

class StoreManager; // Попередня декларація

/**
 * @brief Клас SaleProcessor. Керує логікою оформлення покупки (виписка чека)
 * та взаємодіє зі StoreManager для коректування бази.
 */
class SaleProcessor final
{
public:
    SaleProcessor(StoreManager& manager);
    
    /**
     * @brief Оформляє повну покупку з кількох товарів.
     * @param cashierName Ім'я касира (поточний користувач).
     */
    void ProcessFullSale(const std::string& cashierName);

private:
    StoreManager& m_storeManager;
    
    int getNextReceiptId() const { static int id = 1000; return ++id; } 
};