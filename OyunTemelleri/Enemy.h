#pragma once // Bu kalkan, dosyanın projede kazara iki kere okunmasını engeller
#include <string>

class Enemy {   // düşman sınıfı
public:
    Enemy(std::string, double);
    void takeDamage(double);
    bool isAlive() const { return health > 0; }
    double getAttackPower() const;
    std::string getName() const { return name; }
    double getHealth() const { return health; }

private:
    std::string name;
    double health;
    double attackPower = 15.0;

};