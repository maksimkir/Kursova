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

    void AddObject();
    void EditObject();
    void DeleteObject();
    void ViewObjects() const;

    void SearchObjects() const;
    void SortObjects();
    void FilterObjects() const;
    void PerformInventory() const;
    void DisplayHelp() const;

    StoreItem* GetItemForSale(const std::string& name);

private:
    ProductCollection m_products;
    const std::string m_productFile;


    bool LoadProducts();
    bool SaveProducts() const;


    StoreItem* FindStoreItemByName(const std::string& name);
    void InternalSort(std::function<bool(const ProductPointer&, const ProductPointer&)> comparator);
};