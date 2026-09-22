#pragma once
#include "Item.h"
#include <string>

class Consumable : public Item {
public:
    Consumable(std::string, double, double);
    double getAmount() const { return healAmount; }
    void printDetails() const override;

private:
    double healAmount;

};