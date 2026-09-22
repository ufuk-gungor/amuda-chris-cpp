#include "Weapon.h"
#include <print>
#include <cstdlib>

Weapon::Weapon(std::string n, double w, int a, double d, int ac) : Item{ n, w }, ammo{ a }, damage{ d }, accuracy{ ac } {}


double Weapon::fire() {
    if (ammo > 0) {
        ammo--;
        std::println("{} fired.", Item::getName());
        int dice = rand() % 100 + 1;
        if (dice <= accuracy) {
            double f_dam = (rand() % 11 - 5.0) + damage;
            if (f_dam < 0) {
                f_dam = 0;
            }
            return f_dam;
        }
        else {
            std::println("Shot missed!");
            return 0.0;
        }
    }
    else {
        std::println("No bullets!");
        return 0.0;
    }
}


void Weapon::printDetails() const {
    std::println("{} - {} - {} bullets - {} damage", Item::getName(), Item::getWeight(), ammo, damage);
}