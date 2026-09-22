#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Item.h"
class Weapon;
class Enemy;
class Location;

enum class CharacterState {
    Idle,
    Sprinting,
    Climbing
};

class MainCharacter {
public:
    MainCharacter();
    void showInfo() const;
    void takeDamage(double damage);
    void setClimbing(bool isClimbing);
    void updateGame();
    void setRunning(bool isRunning);
    bool pickUpItem(std::unique_ptr<Item>& new_item);
    std::unique_ptr<Item> removeItemAtIndex(int);
    void equipFirstWeapon();
    bool changeWeapon();
    bool attack(Enemy* target);
    bool saveGame() const;
    bool loadGame();
    void useItem(int index);
    int getInventorySize() const { return static_cast<int>(inventory.size()); }
    Item* getItem(int index) const { return inventory[index].get(); }
    double getHealth() const { return health; }
    double getStamina() const { return stamina; }
    Location* getCurrentLocation() const { return currentLocation; }
    void setCurrentLocation(Location* newLoc) { currentLocation = newLoc; }
    std::string getSavedRoomName() const { return savedRoomName; }

private:
    std::string name = "Amuda_Chris";
    static constexpr double maxHealth{ 300.0 };
    double health;
    double stamina;
    CharacterState currentState;
    std::vector<std::unique_ptr<Item>> inventory;
    double currentCarryWeight;
    double maxCarryWeight;
    Weapon* equippedWeapon = nullptr;
    Location* currentLocation = nullptr;
    std::string savedRoomName;
};