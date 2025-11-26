#pragma once
#include "IPrintable.h"
#include <string>

class Product : public IPrintable
{
public:
    virtual std::string GetName() const = 0;
    virtual void SetName(const std::string& name) = 0;
    virtual double GetPrice() const = 0;
    virtual void SetPrice(double price) = 0;

    virtual std::string ToCSVString() const = 0;

    virtual void DisplayInfo() const override = 0;
    
    virtual ~Product() = default;
};