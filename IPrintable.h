#pragma once

#include <iostream>

class IPrintable
{

public:
    virtual void DisplayInfo() const = 0;

    virtual ~IPrintable() = default;
};