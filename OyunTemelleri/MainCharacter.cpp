#include "MainCharacter.h"
#include "Weapon.h"
#include "Consumable.h"
#include "Enemy.h"
#include "Location.h"
#include <iostream>
#include <print>
#include <fstream>
#include <limits>
#include <memory>
#include <utility>

MainCharacter::MainCharacter() : health{ maxHealth }, stamina{ 100 }, currentState{ CharacterState::Idle }, currentCarryWeight{ 0.0 }, maxCarryWeight{ 30.0 } {}  // Constructor


void MainCharacter::showInfo() const {
    std::string cur;
    if (currentState == CharacterState::Idle) {
        cur = "Idle";
    }
    else if (currentState == CharacterState::Sprinting) {
        cur = "Sprinting";
    }
    else {
        cur = "Climbing";
    }
    std::println("Name: {}\nState: {}", name, cur);
    if (currentLocation != nullptr) {
        std::println("Current location: {}", currentLocation->getName());
    }
    if (equippedWeapon != nullptr) {
        std::println("Equipped weapon: {}", equippedWeapon->getName());
    }
    std::cout << "Inventory size: " << inventory.size() << "\n";
    std::println("Inventory capacity: {} / {}", currentCarryWeight, maxCarryWeight);
    if (!inventory.empty()) {
        for (int i = 0; i < static_cast<int>(inventory.size()); i++) {
            std::cout << i + 1 << ". ";
            inventory[i]->printDetails();
        }
    }
    std::cout << "---------------------------" << std::endl;
}


void MainCharacter::takeDamage(double damage) {
    if (damage >= health) {
        health = 0;
        std::println("The end of A.C.");
    }
    else {
        health -= damage;
    }
}


void MainCharacter::setClimbing(bool isClimbing) {
    if (isClimbing) {
        currentState = CharacterState::Climbing;
    }
    else {
        currentState = CharacterState::Idle;
    }
}


void MainCharacter::updateGame() {
    if (currentState == CharacterState::Idle) {
        if (stamina < 100) {  // Increase it by 10 until 100
            stamina += 10;
            if (stamina > 100) {
                stamina = 100; 
            }
        }
    }
    else if (currentState == CharacterState::Sprinting) {
        if (stamina >= 15) {
            stamina -= 15; // He can run if he has stamina
        }
        currentState = CharacterState::Idle;
    }
    else if (currentState == CharacterState::Climbing) {
        if (stamina >= 20) {
            stamina -= 20; 
        }
        else {
            // Insufficient energy causes fall damage
            std::println("No energy! A.Chris took fall damage.");
            takeDamage(30);
        }
        currentState = CharacterState::Idle; 
    }
}


void MainCharacter::setRunning(bool isRunning) {
    if (isRunning) {
        currentState = CharacterState::Sprinting;
    }
    else {
        currentState = CharacterState::Idle;
    }
}


bool MainCharacter::pickUpItem(std::unique_ptr<Item>& new_item) {
    if (currentCarryWeight + new_item->getWeight() <= maxCarryWeight) {
        double weight = new_item->getWeight();
        inventory.push_back(std::move(new_item));
        currentCarryWeight += weight;
        return true;
    }
    else {
        std::println("Insufficient capacity!");
        return false;
    }
}


std::unique_ptr<Item> MainCharacter::removeItemAtIndex(int index) {
    if (index < 0 || index >= static_cast<int>(inventory.size())) {
        return nullptr;
    }
    else {
        if (inventory[index].get() == equippedWeapon && equippedWeapon != nullptr) {
            equippedWeapon = nullptr;
        }
        double weight = inventory[index]->getWeight();
        std::unique_ptr<Item> removed = std::move(inventory[index]);
        currentCarryWeight -= weight;
        inventory.erase(inventory.begin() + index);
        return removed;
    }
}


void MainCharacter::equipFirstWeapon() {
    equippedWeapon = nullptr;
    for (int i = 0; i < getInventorySize(); i++) {
        // Item pointer'ının gerçekten bir Weapon nesnesini gösterip göstermediğini runtime'da kontrol ediyoruz.
        Weapon* chosenGun = dynamic_cast<Weapon*>(inventory[i].get());

        if (chosenGun != nullptr) { // Eğer dönüştürme başarılıysa (Yani bu gerçekten bir silahsa)
            equippedWeapon = chosenGun;
            std::println("{} is equipped", equippedWeapon->getName());
            return;
        }
    }
    std::println("You do not have any guns in your inventory!");
}


bool MainCharacter::changeWeapon() {
    std::vector<Weapon*> weapons;
    for (int i = 0; i < MainCharacter::getInventorySize(); i++) {
        Weapon* wp = dynamic_cast<Weapon*>(inventory[i].get());
        if (wp != nullptr) {
            weapons.push_back(wp);
        }
    }
    
    if (static_cast<int>(weapons.size()) == 0) {
        std::println("You have no weapons!");
        return false;
    }
    else if (static_cast<int>(weapons.size()) == 1) {
        if (equippedWeapon != weapons[0]) {
            equippedWeapon = weapons[0];
            std::println("{} is equipped.", weapons[0]->getName());
            return true;
        }
        else {
            std::println("You have only 1 weapon!");
            return false;
        }
    }

    std::println("Choose the weapon: ");

    for (int i = 0; i < static_cast<int>(weapons.size()); i++) {
        std::println("{}. {}", i + 1, weapons[i]->getName());
    }
    
    int answer;
    while (true) {
        std::cin >> answer;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            std::println("Invalid input. Please enter a number between {} and {}.", 1, static_cast<int>(weapons.size()));
        }
        else if (answer < 1 || answer > static_cast<int>(weapons.size())) {
            std::println("Invalid choice. Please enter a number between {} and {}.", 1, static_cast<int>(weapons.size()));
        }
        else {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            break;
        }
    }
    
    equippedWeapon = weapons[answer - 1];
    std::println("Equipped weapon changed to {}", weapons[answer - 1]->getName());
    return true;
}


bool MainCharacter::attack(Enemy* target) {  //parametre olarak enemy alıyorum ki onun take damage fonksiyonunu kullanabileyim
    if (equippedWeapon == nullptr) {
        equipFirstWeapon();
    }

    if (equippedWeapon == nullptr) {
        std::println("No weapon equipped!");
        return false;
    }
    double damage = equippedWeapon->fire();
    if (damage > 0) {
        target->takeDamage(damage);
    }
    return true;
}


bool MainCharacter::saveGame() const {
    if (currentLocation == nullptr) {
        std::println("[ERROR] No location detected!");
        return false;
    }
    std::ofstream saveFile("savegame.txt"); // Dosyayı yazmak için aç
    
    if (saveFile.is_open()) {
        saveFile << name << "\n";
        saveFile << health << "\n";
        saveFile << stamina << "\n";
        saveFile << currentLocation->getName() << "\n"; // Odanın adını kaydet
        saveFile << inventory.size() << "\n"; // Kaç eşya var?

        // Çantayı dönüyoruz
        for (int i = 0; i < static_cast<int>(inventory.size()); i++) {
            // dynamic_cast ile çantadaki şeyin SİLAH olup olmadığına bakıyoruz
            Weapon* wpn = dynamic_cast<Weapon*>(inventory[i].get());

            if (wpn != nullptr) {
                // Eğer silahsa, başına WEAPON etiketi koy ve 5 özelliği yazdır:
                saveFile << "WEAPON " << wpn->getName() << " " << wpn->getWeight() << " " << wpn->getAmmo() << " " << wpn->getDamage() << " " << wpn->getAccuracy() << "\n";
            }
            else {
                // İksir de olabilir ona CONSUMABLE etikeyi koy ve 3 özelliği yazdır:
                Consumable* cons = dynamic_cast<Consumable*>(inventory[i].get());

                if (cons != nullptr) {
                    saveFile << "CONSUMABLE " << cons->getName() << " " << cons->getWeight() << " " << cons->getAmount() << "\n";
                }
                else {
                    // Eğer silah değilse (normal eşyaysa), ITEM etiketi koy ve 2 özelliği yazdır:
                    saveFile << "ITEM " << inventory[i]->getName() << " " << inventory[i]->getWeight() << "\n";
                }
            }
        }
        if (equippedWeapon != nullptr) {
            saveFile << "EQUIPPED " << equippedWeapon->getName() << "\n";
        }
        else {
            saveFile << "EQUIPPED NONE" << "\n";
        }
        saveFile.close();
        if (!saveFile) {
            std::println("[ERROR] Game is not saved!");
            return false;
        }
        std::println("[SYSTEM] Game saved successfully.");
        return true;
    }
    else {
        std::println("[ERROR] Game is not saved!");
        return false;
    }
}


bool MainCharacter::loadGame() {
    std::ifstream loadFile("savegame.txt"); // Open it to read the file
    std::string t_name;  // temp name
    double t_health, t_stamina;  // temp health, stamina
    std::string t_savedRoomName; // temp saved room name
    std::vector<std::unique_ptr<Item>> t_inventory; // temp inventory
    double t_weight = 0.0; // temp weight
    std::string t_ew_name; // temp equipped weapon name


    if (loadFile.is_open()) {
        if (!(loadFile >> t_name >> t_health >> t_stamina)) {  
            return false;
        }
        if (t_health < 0 || t_health > maxHealth || t_stamina < 0 || t_stamina > 100) {
            return false;
        }
        if (!(std::getline(loadFile >> std::ws, t_savedRoomName))) { 
            return false;
        }

        int loadedSize;
        if (!(loadFile >> loadedSize)) { // Size of the inventory
            return false;
        }
        if (loadedSize < 0) {
            return false;
        }

        for (int i = 0; i < loadedSize; i++) {
            std::string tag;
            // Read the first word (LABEL)!
            if (!(loadFile >> tag)) {
                return false;
            }

            if (tag == "WEAPON") {
                std::string wName;
                double wWeight, wDamage;
                int wAmmo, wAccuracy;

                if (!(loadFile >> wName >> wWeight >> wAmmo >> wDamage >> wAccuracy)) {
                    return false;
                }
                if (wWeight < 0 || wAmmo < 0 || wDamage < 0 || wAccuracy < 0 || wAccuracy > 100) {
                    return false;
                }
                std::unique_ptr<Weapon> temp = std::make_unique<Weapon>(wName, wWeight, wAmmo, wDamage, wAccuracy);
                t_inventory.push_back(std::move(temp));
                t_weight += wWeight; // Update the weight
            }
            else if (tag == "CONSUMABLE") {
                std::string cName;
                double cWeight, cAmount;

                if (!(loadFile >> cName >> cWeight >> cAmount)) {
                    return false;
                }
                if (cWeight < 0 || cAmount < 0) {
                    return false;
                }
                std::unique_ptr<Consumable> temp = std::make_unique<Consumable>(cName, cWeight, cAmount);
                t_inventory.push_back(std::move(temp));
                t_weight += cWeight; // Update the weight
            }
            else if (tag == "ITEM") {
                std::string iName;
                double iWeight;

                if (!(loadFile >> iName >> iWeight)) {
                    return false;
                }
                if (iWeight < 0) {
                    return false;
                }
                std::unique_ptr<Item> temp = std::make_unique<Item>(iName, iWeight);
                t_inventory.push_back(std::move(temp));
                t_weight += iWeight; // Update the weight
            }
            else {
                return false;
            }
            if (maxCarryWeight < t_weight) {
                return false;
            }
        }
        std::string prefix, weapon;
        if (!(loadFile >> prefix >> weapon)) {
            return false;
        }
        if (prefix != "EQUIPPED") {
            return false;
        }

        bool weaponFound = false;
        if (weapon == "NONE") {
            t_ew_name = "NONE";
        }
        else {
            for (int i = 0; i < loadedSize; i++) {
                // Equipped weapon çantada aranıyor
                Weapon* wpn = dynamic_cast<Weapon*>(t_inventory[i].get());
                if (wpn != nullptr && wpn->getName() == weapon) {
                    weaponFound = true;
                    t_ew_name = wpn->getName();
                    break;
                }
            }
            if (!weaponFound) {
                return false;
            }
        }
        
        // Old equippedWeapon may point into the inventory that is about to be replaced.
        equippedWeapon = nullptr;

        name = t_name;
        health = t_health;
        stamina = t_stamina;
        savedRoomName = t_savedRoomName;
        inventory = std::move(t_inventory);
        currentCarryWeight = t_weight;

        if (t_ew_name != "NONE" && weaponFound) {
            for (int i = 0; i < loadedSize; i++) {
                // Equipped weapon çantada aranıyor
                Weapon* wpn = dynamic_cast<Weapon*>(inventory[i].get());
                if (wpn != nullptr && wpn->getName() == weapon) {
                    equippedWeapon = wpn;
                    break;
                }
            }
        }

        loadFile.close();
        std::println("[SYSTEM] Game loaded successfully.");
        return true;
    }
    else {
        // Dosya yoksa (oyuncu ilk defa oynuyorsa) cokerse diye guvenlik duvari
        std::println("[ERROR] No save file found! Starting a new game.");
        return false;
    }
}


void MainCharacter::useItem(int index) {
    if (index < 0 || index >= getInventorySize()) {
        std::println("[SYSTEM] Invalid index value!");
        return;
    }

    // 1. Increase the health (Upper bound is max health)
    Consumable* con = dynamic_cast<Consumable*>(inventory[index].get());
    if (con == nullptr) {
        std::println("[SYSTEM] This item is not a heal!");
        return;
    }
    health += con->getAmount();

    if (health > maxHealth) {
        health = maxHealth;
    }

    std::println("[SYSTEM] {} is used. New health: {}", inventory[index]->getName(), health);

    // 2. Update the weight
    currentCarryWeight -= inventory[index]->getWeight();

    inventory.erase(inventory.begin() + index);
}