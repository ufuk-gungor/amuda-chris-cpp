#pragma once
#include "Item.h"
#include <string>

class Weapon : public Item {
public:
    Weapon(std::string, double, int, double, int);
    double fire();
    void printDetails() const override;
    int getAmmo() const { return ammo; }
    double getDamage() const { return damage; }
    int getAccuracy() const { return accuracy; }
private:
    int ammo;
    double damage;
    int accuracy;

};