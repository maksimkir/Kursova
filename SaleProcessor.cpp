#include "SaleProcessor.h"
#include "StoreManager.h"
#include "Configuration.h"
#include <iostream>
#include <limits>
#include <iomanip>

SaleProcessor::SaleProcessor(StoreManager& manager) 
    : m_storeManager(manager) {}

void SaleProcessor::ProcessFullSale(const std::string& cashierName)
{
    try {
        Receipt currentReceipt(getNextReceiptId(), cashierName);
        bool addingItems = true;

        std::cout << "\n======== ОФОРМЛЕННЯ ПОКУПКИ (ЧЕК #" << currentReceipt.GetReceiptId() << ") ========" << std::endl;
        
        while (addingItems)
        {
            try {
                std::string name;
                int quantity;
                
                std::cout << "\nВведіть назву товару (або '0' для завершення): ";
                Configuration::ClearInputBuffer(); 
                std::getline(std::cin, name);

                if (name == "0") {
                    addingItems = false;
                    continue;
                }

                quantity = Configuration::GetSafeNumberInput<int>("Введіть кількість для продажу: ");
                
                if (quantity == 0) continue;

                StoreItem* item = m_storeManager.GetItemForSale(name);
                
                if (item) {
                    if (item->GetQuantity() >= quantity) {
                        currentReceipt.AddItem(item->GetName(), item->GetPrice(), quantity);
                        item->UpdateQuantity(-quantity); // Коректування бази
                        std::cout << "// Успіх: Додано " << quantity << " од. " << item->GetName() << " до чека." << std::endl;
                    } else {
                        std::cerr << "// Помилка: Недостатньо '" << item->GetName() << "'. Залишок: " << item->GetQuantity() << " од." << std::endl;
                    }
                } else {
                    std::cerr << "// Помилка: Товар '" << name << "' не знайдено у базі." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "// Помилка при додаванні товару до чека: " << e.what() << std::endl;
                Configuration::s_logger->LogError("Помилка в ProcessFullSale (цикл): " + std::string(e.what()));
            }
        }
        
        if (currentReceipt.CalculateTotal() > 0) {
            currentReceipt.DisplayInfo();
            std::cout << "\n// База наявності товарів скоректована згідно з продажем." << std::endl; 
        } else {
            std::cout << "// Покупку скасовано. Чек порожній." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "// Критична помилка при оформленні покупки: " << e.what() << std::endl;
        Configuration::s_logger->LogError("Критична помилка в ProcessFullSale: " + std::string(e.what()));
    }
}