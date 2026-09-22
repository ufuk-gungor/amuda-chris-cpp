#include "Enemy.h" // Kendi başlık dosyasını çağırıyor
#include <iostream>
#include <print>
#include <cstdlib>

// Kurucu (Constructor) Gövdesi
Enemy::Enemy(std::string n, double h) : name{ n }, health{ h } {}

// Hasar Alma Fonksiyonu Gövdesi
void Enemy::takeDamage(double damage) {
    if (damage < health) {
        health -= damage;
        std::println("{} damage dealt. Remaining health of {}: {}", damage, name, health);
    }
    else {
        health = 0;
        std::println("{} is killed.", name);
    }
}

double Enemy::getAttackPower() const {
    double ad_dam = rand() % 11 - 5;
    if (attackPower + ad_dam < 0) {
        return 0.0;
    }
    else {
        return attackPower + ad_dam;
    }
}