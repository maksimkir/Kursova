#pragma once

#include "Receipt.h"
#include <string>
#include <memory>

class StoreManager;

class SaleProcessor final
{
public:
    SaleProcessor(StoreManager& manager);

    void ProcessFullSale(const std::string& cashierName);

private:
    StoreManager& m_storeManager;
    
    int getNextReceiptId() const { static int id = 1000; return ++id; } 
};