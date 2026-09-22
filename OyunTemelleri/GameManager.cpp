#include "GameManager.h"
#include "Weapon.h"
#include "Consumable.h"
#include "Item.h"
#include "Enemy.h"
#include "StoryManager.h"
#include <print>
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include <memory>
#include <utility>


GameManager::GameManager() 
	: gameContinue{true},
    attacker{ nullptr },
    currentObjectiveStage{ ObjectiveStage::FindSilverKey },
	l1{ nullptr },
	l2{ nullptr },
	l3{ nullptr },
	l4{ nullptr },
	l5{ nullptr }
{
    setupLocations();
    setupItemsAndLocks();
    setupEnemies();
    loadOrStartGame();
}


int GameManager::readIntInRange(int min, int max) const {
    int value{};

    while (true) {
        std::cin >> value;

        // Eğer kullanıcı sayı yerine harf/kelime gibi geçersiz bir input girdiyse
        if (std::cin.fail()) {
            std::cin.clear();
            // cin hata moduna girdiği için önce hata bayrağını temizliyoruz.
            // Bunu yapmazsak sonraki input denemeleri de çalışmaz.

            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            // Hatalı yazılan satırın geri kalanını input buffer'dan siliyoruz.
            // Örneğin kullanıcı "abc" yazdıysa, bu satır abc'yi çöpe atar.

            std::println("Invalid input. Please enter a number between {} and {}.", min, max);
        }

        // Input sayı ama istenen aralığın dışındaysa
        else if (value < min || value > max) {
            std::println("Invalid choice. Please enter a number between {} and {}.", min, max);
        }

        // Input hem sayı hem de istenen aralık içindeyse
        else {
            std::cin.ignore(
                std::numeric_limits<std::streamsize>::max(),
                '\n'
            );
            // Geçerli sayıdan sonra aynı satırda fazladan yazılmış bir şey varsa temizliyoruz.
            // Örneğin kullanıcı "3 abc" yazarsa, 3 alınır ama "abc" buffer'da kalmasın diye silinir.

            return value;
        }
    }
}


bool GameManager::isQuestItem(const std::string& name) const {
    if (name == "Silver_Key" || name == "Gold_Key" || name == "Ancient_Artifact") {
        return true;
    }
    else {
        return false;
    }
}


void GameManager::updateObjectiveAfterPickup(const std::string& itemName) {
    if (itemName == "Silver_Key" && !hasInventoryItem("Gold_Key")) {
        currentObjectiveStage = ObjectiveStage::ReachMercenaryCamp;
    }
    else if (itemName == "Gold_Key") {
        currentObjectiveStage = ObjectiveStage::UnlockTemple;
    }
    else if (itemName == "Mercenary_Note" && !hasInventoryItem("Gold_Key")) {
        currentObjectiveStage = ObjectiveStage::FindGoldKey;
    }
    else if (itemName == "Ancient_Artifact") {
        currentObjectiveStage = ObjectiveStage::EscapeRuins;
    } 
    else {
        return;
    }
}


void GameManager::updateObjectiveAfterMove(Location* room) {
    if (currentObjectiveStage == ObjectiveStage::ReachMercenaryCamp && room->getName() == "Mercenary Camp") {
        currentObjectiveStage = ObjectiveStage::SearchCamp;
    }
    else if (currentObjectiveStage == ObjectiveStage::UnlockTemple && room->getName() == "Ancient Temple") {
        currentObjectiveStage = ObjectiveStage::DefeatLazarevic;
    }
    else if (currentObjectiveStage == ObjectiveStage::EscapeRuins && room->getName() == "Hidden Cave") {
        story_manager.printEndingStory();
        std::println("\nPress Enter to exit...");
        std::cin.get();
        gameContinue = false;
    }
}



void GameManager::printExitInfo(std::string directionName, Location* destination) const {
    if (destination != nullptr) {
        std::string isLock;
        if (destination->getLock() && !hasKeyFor(destination)) {
            std::string key = destination->getKey();
            isLock = "[LOCKED: " + key + " is required.]";
        }
        else {
            isLock = "[OPEN]";
        }
        std::println("{} -> {} {}", directionName, destination->getName(), isLock);
    }
}


bool GameManager::hasInventoryItem(const std::string& item) const {
    for (int i = 0; i < chris.getInventorySize(); i++) {
        if (item == chris.getItem(i)->getName()) {
            return true;
        }
    }
    return false;
}


Location* GameManager::findLocationByName(const std::string& roomName) const {
    for (int i = 0; i < static_cast<int>(locations.size()); i++) {
        if (roomName == locations[i]->getName()) {
            return locations[i].get();
        }
    }
    return nullptr;
}


void GameManager::move(Location* room) {
    chris.setCurrentLocation(room);
    std::println("You moved to {}.", room->getName());
    updateObjectiveAfterMove(room);
    if (!gameContinue) {
        return;
    }
    if (!hasVisited(room)) {
        markVisitedRoom(room);
        story_manager.printFirstVisitStory(room);
        story_manager.printEnemyEncounterStory(room);
    }
}


bool GameManager::hasKeyFor(Location* destination) const {
    if (destination == nullptr) {
        return false;
    }
    else {
        if (!destination->getLock()) {
            return true;
        }
        else {
            for (int i = 0; i < chris.getInventorySize(); i++) {
                if (chris.getItem(i)->getName() == destination->getKey()) {
                    return true;
                }
            }
            return false;
        }
    }
}


bool GameManager::canMoveTo(Location* destination) const {
    if (!hasKeyFor(destination)) {
        return false;
    }
    else {
        return true;
    }
}


void GameManager::printMenu() const {
    std::println("\n===========================");
    std::println("What do you want to do?");
    std::println("1 - Show character data");
    std::println("2 - Attack the enemy");
    std::println("3 - Drink potion");
    std::println("4 - Save and quit");
    std::println("5 - Move to another location");
    std::println("6 - Search the room");
    std::println("7 - Drop item");
    std::println("8 - Change equipped weapon");
    std::println("9 - Inspect current room");
    std::println("===========================");
    std::cout << "Choice: ";
}


void GameManager::printStatus() const {
    std::println("\n--- [ HP: {} | STAMINA: {} ] ---", chris.getHealth(), chris.getStamina());
    std::println("Objective: {}", getObjectiveText(currentObjectiveStage));
}


bool GameManager::hasVisited(Location* loc) const {
    for (int i = 0; i < static_cast<int>(visitedRooms.size()); i++) {
        if (loc == visitedRooms[i]) {
            return true;
        }
    }
    return false;
}


void GameManager::markVisitedRoom(Location* loc) {
    for (int i = 0; i < static_cast<int>(visitedRooms.size()); i++) {
        if (loc == visitedRooms[i]) {
            return;
        }
    }
    visitedRooms.push_back(loc);
}


std::string GameManager::getObjectiveText(ObjectiveStage stage) const {
    switch (stage) {
        case ObjectiveStage::FindSilverKey:
            return "Find the Silver Key.";
        case ObjectiveStage::ReachMercenaryCamp:
            return "Reach the Mercenary Camp.";
        case ObjectiveStage::SearchCamp:
            return "Search the camp and prepare for the temple.";
        case ObjectiveStage::FindGoldKey:
            return "Find the Gold Key on the Cliff Path.";
        case ObjectiveStage::UnlockTemple:
            return "Unlock the Ancient Temple.";
        case ObjectiveStage::DefeatLazarevic:
            return "Defeat Lazarevic.";
        case ObjectiveStage::RetrieveArtifact:
            return "Retrieve the Ancient Artifact.";
        case ObjectiveStage::EscapeRuins:
            return "Return to the Hidden Cave and escape the ruins.";
    }
    return "";
}


void GameManager::handleEnemyCounterAttack() {
    std::vector<std::unique_ptr<Enemy>>& target = chris.getCurrentLocation()->getEnemies();
    Enemy* mainTarget = nullptr;
    if (attacker != nullptr && attacked) {
        mainTarget = attacker;
    }
    else if (static_cast<int>(target.size()) != 0 && !attacked) {
        mainTarget = target[0].get();
    }
    if (mainTarget != nullptr) {
        double dam = mainTarget->getAttackPower();
        chris.takeDamage(dam);
        std::println("{} attacks you for {} damage! Chris remaining health {}", mainTarget->getName(), dam, chris.getHealth());
        if (chris.getHealth() == 0) {
            std::println("\n========================");
            std::println("GAME OVER");
            std::println("That is the end of the road for Chris");
            std::println("========================");
            gameContinue = false;
        }
    }
    attacker = nullptr;
    attacked = false;
}


bool GameManager::handleAttack() {
    std::vector<std::unique_ptr<Enemy>>& target = chris.getCurrentLocation()->getEnemies();
    Enemy* mainTarget = nullptr;
    int index {};
    if (static_cast<int>(target.size()) == 0) {
        std::println("This area is safe. There is no one to attack.");
        return false;
    }
    else if (static_cast<int>(target.size()) == 1) {
        mainTarget = target[0].get();
        index = 0;
    }
    else {
        for (int i = 0; i < static_cast<int>(target.size()); i++) {
            std::println("{}. {}", i + 1, target[i]->getName());
        }
        std::println("Choose enemy to attack: ");
        int ans = readIntInRange(1, static_cast<int>(target.size()));
        mainTarget = target[ans - 1].get();
        index = ans - 1;
    }

    if (mainTarget == nullptr) {
        return false;
    }
    bool attackHappened = chris.attack(mainTarget);

    if (!attackHappened) {
        return false;
    }
    else {
        attacked = true;
    }
    if (!mainTarget->isAlive()) {
        story_manager.printStoryAfterEnemyKilled(mainTarget);
        // Boss defeat advances the objective and drops the artifact.
        if (mainTarget->getName() == "Lazarevic") {
            currentObjectiveStage = ObjectiveStage::RetrieveArtifact;
            auto artifact = std::make_unique<Item>("Ancient_Artifact", 2.0);
            chris.getCurrentLocation()->addItem(std::move(artifact));
            std::println("As Lazarevic collapsed to the ground, an ancient, gleaming object fell from his hand...");
        }
        target.erase(target.begin() + index);
    }
    else {
        attacker = mainTarget;
    }
    return true;
}


bool GameManager::handleUsePotion() {
    std::vector<Consumable*> consumables;
    std::vector<int> potionIndexes;
    // Çantanın başından sonuna kadar tek tek bakıyoruz
    for (int i = 0; i < chris.getInventorySize(); i++) {

        // İlgili indeksteki eşyayı İksir (Consumable) kalıbına sokmaya çalışıyoruz
        Consumable* cons = dynamic_cast<Consumable*>(chris.getItem(i));

        if (cons != nullptr) {
            consumables.push_back(cons);
            potionIndexes.push_back(i);
        }
    }
    if (static_cast<int>(consumables.size()) == 0) {
        std::println("[SYSTEM] There is no potion!");
        return false;
    }
    else if (static_cast<int>(consumables.size()) == 1) {
        chris.useItem(potionIndexes[0]);
        return true;
    }
    
    for (int i = 0; i < static_cast<int>(consumables.size()); i++) {
        std::println("{}. {}", i + 1, consumables[i]->getName());
    }
    std::println("Choose potion number: ");
    int ans = readIntInRange(1, static_cast<int>(consumables.size()));
    chris.useItem(potionIndexes[ans - 1]);
    return true;
}


bool GameManager::handleMovement() {
    Location* current = chris.getCurrentLocation();
    if (current->getNorth() != nullptr) {
        std::println("1. Go North ({})", current->getNorth()->getName());
    }
    if (current->getSouth() != nullptr) {
        std::println("2. Go South ({})", current->getSouth()->getName());
    }
    if (current->getEast() != nullptr) {
        std::println("3. Go East ({})", current->getEast()->getName());
    }
    if (current->getWest() != nullptr) {
        std::println("4. Go West ({})", current->getWest()->getName());
    }
    std::cout << "Choice: ";
    int ans = readIntInRange(1, 4);
    Location* destination = nullptr;

    switch (ans) {
    case 1:
        destination = current->getNorth();
        break;
    case 2:
        destination = current->getSouth();
        break;
    case 3:
        destination = current->getEast();
        break;
    case 4:
        destination = current->getWest();
        break;
    default:
        std::println("Enter a valid number. (1 - 4)");
        return false;
    }

    if (destination == nullptr) {
        std::println("You can not move that way!");
        return false;
    }
    else if (!canMoveTo(destination)) {
        story_manager.printLockedDoorStory(destination);
        return false;
    }
    std::println("How do you want to travel?");
    std::println("1. Walk normally (Recovers stamina)");
    std::println("2. Sprint (Uses 15 stamina, fast)");
    std::println("3. Climb the obstacles (Uses 20 stamina)");
    std::cout << "Pace: ";

    int paceAnswer = readIntInRange(1, 3);
    if (paceAnswer == 2) {
        if (chris.getStamina() < 15) {
            std::println("Not enough stamina to sprint.");
            return false;
        }
        chris.setRunning(true);
    }
    else if (paceAnswer == 3) {
        chris.setClimbing(true);
    }
    else {
        chris.setRunning(false);
        chris.setClimbing(false);
    }
    move(destination);
    return true;
}


bool GameManager::handleSearching() {
    Location* current = chris.getCurrentLocation();
    std::vector<std::unique_ptr<Item>>& roomItems = current->getItems();

    if (roomItems.size() == 0) {
        std::println("No item found!");
        return false;
    }
    else {
        // Iterate backwards so erasing items does not invalidate upcoming indices.
        for (int i = static_cast<int>(roomItems.size()) - 1; i >= 0; i--) {

            Item* pickedItem = roomItems[i].get();
            if (chris.pickUpItem(roomItems[i])) {
                std::println("You picked up: {}", pickedItem->getName());
                story_manager.printStoryAfterPickup(pickedItem);
                updateObjectiveAfterPickup(pickedItem->getName());
                roomItems.erase(roomItems.begin() + i); 
            }
            else {
                std::println("You had to leave {} on the ground.", roomItems[i]->getName());
            }
        }
    }
    return true;
}


void GameManager::handleDropItem() {
    if (chris.getInventorySize() == 0) {
        std::println("Your inventory is empty.");
        return;
    }
    else {
        for (int i = 0; i < chris.getInventorySize(); i++) {
            std::println("{}. {}", i + 1, chris.getItem(i)->getName());
        }
        std::println("Dropped item number: ");
    }
    int ans = readIntInRange(1, chris.getInventorySize());
    if (isQuestItem(chris.getItem(ans - 1)->getName())) {
        std::println("This quest item cannot be dropped.");
        return;
    }

    std::unique_ptr<Item> dropped = chris.removeItemAtIndex(ans - 1);
    if (dropped != nullptr) {
        std::println("Dropping {}", dropped->getName());
        chris.getCurrentLocation()->addItem(std::move(dropped));
    }
}


bool GameManager::handleChangeWeapon() {
    return chris.changeWeapon();
}


void GameManager::handleInspectRoom() const {
    const Location* currentLoc = chris.getCurrentLocation();
    std::println("Name: {}\nInfo: {}", currentLoc->getName(), currentLoc->getDescription());
    const std::vector<std::unique_ptr<Item>>& items = currentLoc->getItems();
    if (static_cast<int>(items.size()) == 0) {
        std::println("No visible items.");
    }
    else {
        std::println("ITEMS: ");
        for (int i = 0; i < static_cast<int>(items.size()); i++) {
            std::println("{}. {}", i + 1, items[i]->getName());
        }
    }
    const std::vector<std::unique_ptr<Enemy>>& enemies = currentLoc->getEnemies();
    if (static_cast<int>(enemies.size()) == 0) {
        std::println("No enemies here.");
    }
    else {
        std::println("ENEMIES: ");
        for (int i = 0; i < static_cast<int>(enemies.size()); i++) {
            std::println("{}. {} - {} HP", i + 1, enemies[i]->getName(), enemies[i]->getHealth());
        }
    }
    std::println("EXITS: ");
    printExitInfo("North", currentLoc->getNorth());
    printExitInfo("South", currentLoc->getSouth());
    printExitInfo("East", currentLoc->getEast());
    printExitInfo("West", currentLoc->getWest());
}


bool GameManager::handlePlayerChoice(int answer) {
    if (answer == 1) {
        chris.showInfo();
    }
    else if (answer == 2) {
        return handleAttack();  
    }
    else if (answer == 3) {
        return handleUsePotion();
    }
    else if (answer == 4) {
        if (chris.saveGame() && saveWorldState()) {
            std::println("Quitting...");
            gameContinue = false;
        }
        else {
            std::println("Save failed. Game will continue.");
        }
    }
    else if (answer == 5) {
        return handleMovement();
    }
    else if (answer == 6) {
        return handleSearching();
    }
    else if (answer == 7) {
        handleDropItem();
    }
    else if (answer == 8) {
        return handleChangeWeapon();
    }
    else if (answer == 9) {
        handleInspectRoom();
    }
    else {
        std::println("Invalid choice. Write a number between 1 - 9.");
    }
    return false;
}


void GameManager::setupLocations() {
    locations.push_back(std::make_unique<Location>("Jungle Ruins", "Warm but dangerous place with ruins"));
    locations.push_back(std::make_unique<Location>("Hidden Cave", "A mysterious and dark place"));
    locations.push_back(std::make_unique<Location>("Mercenary Camp", "Perfect area for action"));
    locations.push_back(std::make_unique<Location>("Cliff Path", "A windy, dangerous and narrow path"));
    locations.push_back(std::make_unique<Location>("Ancient Temple", "A golden, huge temple"));

    // Non-owning pointers to locations owned by the vector.
    l1 = locations[0].get();
    l2 = locations[1].get();
    l3 = locations[2].get();
    l4 = locations[3].get();
    l5 = locations[4].get();

    // Connect locations.
    l1->setConnections(l2, nullptr, l3, nullptr);
    l2->setConnections(l3, l1, nullptr, l4);
    l3->setConnections(l5, l2, nullptr, l1);
    l4->setConnections(nullptr, nullptr, l2, nullptr);
    l5->setConnections(nullptr, l3, nullptr, nullptr);
}


void GameManager::setupItemsAndLocks() {
    // Create world items.
    auto i1 = std::make_unique<Item>("Strange_Vial", 3);
    auto i2 = std::make_unique<Item>("Silver_Key", 5);
    auto i3 = std::make_unique<Item>("Gold_Key", 12);
    auto i4 = std::make_unique<Item>("Mercenary_Note", 0.5);
    auto w1 = std::make_unique<Weapon>("AK-47", 5, 30, 15.5, 80);
    auto c1 = std::make_unique<Consumable>("Heal", 2, 70);
    auto c2 = std::make_unique<Consumable>("First_Aid_Kit", 4, 85);

    // Place items and configure locked locations.
    l1->addItem(std::move(i1));
    l2->addItem(std::move(i2));
    l3->addItem(std::move(i4));
    l3->addItem(std::move(c1));
    l3->addItem(std::move(w1));
    l4->addItem(std::move(i3));
    l4->addItem(std::move(c2));
    l3->setKey("Silver_Key");
    l5->setKey("Gold_Key");
}


void GameManager::setupEnemies() {
    // Create enemies.
    auto en1 = std::make_unique<Enemy>("Lazarevic", 200);
    auto en2 = std::make_unique<Enemy>("Mercenary", 100);
    auto en3 = std::make_unique<Enemy>("Talbot", 50);

    // Place enemies in the world.
    l1->addEnemy(std::move(en2));
    l1->addEnemy(std::move(en3));
    l5->addEnemy(std::move(en1));
}


void GameManager::loadOrStartGame() {
    // 6. Oyun yükleme kısmını aktive et 
    if (chris.loadGame()) { // Eğer kayıt varsa ve başarıyla yüklendiyse
        std::string room = chris.getSavedRoomName(); // Dosyadaki oda adını çek
        
        /*
        worldsave yoksa:
            elimizde sadece inventory bilgisi var
            default world + inventory cleanup yap

        worldsave varsa:
            elimizde gerçek world bilgisi var
            odaları temizle
            worldsave'deki itemları yeniden yerleştir
        */
        if (!loadWorldState()) {
            removeCollectedItemsFromWorld();
        }

        Location* loc = findLocationByName(room);
        if (loc != nullptr) {
            chris.setCurrentLocation(loc);
            markVisitedRoom(loc);
        }
        else {
            std::println("[SYSTEM] Room is not found. Sending you to {}", l2->getName());
            chris.setCurrentLocation(l2);
            markVisitedRoom(l2);
        }
    }
    else { // Eğer kayıt yoksa (İlk defa oynanıyorsa)
        std::unique_ptr<Item> startingWeapon = std::make_unique<Weapon>("Desert_Eagle", 2.0, 10, 35.0, 60);
        chris.pickUpItem(startingWeapon);
        chris.equipFirstWeapon();
        chris.setCurrentLocation(l2);
        markVisitedRoom(l2);
        story_manager.printFirstVisitStory(l2);
    }
}


void GameManager::removeCollectedItemsFromRoom(Location* loc) {
    std::vector<std::unique_ptr<Item>>& roomItems = loc->getItems();
    for (int i = static_cast<int>(roomItems.size()) - 1; i >= 0; i--) {
        for (int j = 0; j < static_cast<int>(chris.getInventorySize()); j++) {
            if (roomItems[i]->getName() == chris.getItem(j)->getName()) {
                roomItems.erase(roomItems.begin() + i);
                break;
            }
        }
    }
}


void GameManager::removeCollectedItemsFromWorld() {
    for (int i = static_cast<int>(locations.size()) - 1; i >= 0; i--) {
        removeCollectedItemsFromRoom(locations[i].get());
    }
}


void GameManager::clearAllRoomItems() {
    for (int i = 0; i < static_cast<int>(locations.size()); i++) {
        std::vector<std::unique_ptr<Item>>& items = locations[i]->getItems();
        items.clear();
    }
}


void GameManager::clearAllRoomEnemies() {
    for (int i = 0; i < static_cast<int>(locations.size()); i++) {
        std::vector<std::unique_ptr<Enemy>>& enemies = locations[i]->getEnemies();
        enemies.clear();
    }
}


bool GameManager::saveWorldState() const {
    std::ofstream saveFile("worldsave.txt"); // Dosyayı yazmak için aç

    if (saveFile.is_open()) {
        // 0. Version sayısını yaz
        saveFile << "VERSION 1" << "\n";

        // 1. Düşman sayısını yaz
        saveFile << "ENEMIES " << locations.size() << "\n";

        // 2. Odalardaki düşman sayısı ve düşman isim - canlarını yaz
        for (int i = 0; i < static_cast<int>(locations.size()); i++) {
            std::vector<std::unique_ptr<Enemy>>& enemies = locations[i]->getEnemies();
            saveFile << i << " " << enemies.size() << "\n";

            for (int j = 0; j < static_cast<int>(enemies.size()); j++) {
                saveFile << enemies[j]->getName() << " " << enemies[j]->getHealth() << "\n";
            }
        }

        // 3. Oda sayısını yaz
        saveFile << "ROOMS " << locations.size() << "\n";

        // 4. Oda indeksini, item sayısını varsa itemleri yaz
        for (int i = 0; i < static_cast<int>(locations.size()); i++) {
            std::vector<std::unique_ptr<Item>>& roomItems = locations[i]->getItems();
            saveFile << i << " " << roomItems.size() << "\n";
            for (int j = 0; j < static_cast<int>(roomItems.size()); j++) {
                // dynamic_cast ile odadaki şeyin SİLAH olup olmadığına bakıyoruz
                Weapon* wpn = dynamic_cast<Weapon*>(roomItems[j].get());

                if (wpn != nullptr) {
                    // Eğer silahsa, başına WEAPON etiketi koy ve 5 özelliği yazdır:
                    saveFile << "WEAPON " << wpn->getName() << " " << wpn->getWeight() << " " << wpn->getAmmo() << " " << wpn->getDamage() << " " << wpn->getAccuracy() << "\n";
                }
                else {
                    // İksir de olabilir ona CONSUMABLE etikeyi koy ve 3 özelliği yazdır:
                    Consumable* cons = dynamic_cast<Consumable*>(roomItems[j].get());

                    if (cons != nullptr) {
                        saveFile << "CONSUMABLE " << cons->getName() << " " << cons->getWeight() << " " << cons->getAmount() << "\n";
                    }
                    else {
                        // Eğer silah değilse (normal eşyaysa), ITEM etiketi koy ve 2 özelliği yazdır:
                        saveFile << "ITEM " << roomItems[j]->getName() << " " << roomItems[j]->getWeight() << "\n";
                    }
                }
            }
        }

        // 5. Ziyaret edilen oda indexlerini yaz
        saveFile << "VISITED " << visitedRooms.size() << "\n";
        for (int i = 0; i < static_cast<int>(visitedRooms.size()); i++) {
            for (int j = 0; j < static_cast<int>(locations.size()); j++) {
                if (visitedRooms[i] == locations[j].get()) {
                    saveFile << j << "\n";
                    break;
                }
            }
        }

        // 6. Objectivei yaz
        saveFile << "OBJECTIVE_STAGE " << static_cast<int>(currentObjectiveStage) << "\n";

        // 7. Başarılıysa true döndür
        saveFile.close();
        if (!saveFile) {
            std::println("[ERROR] World is not saved!");
            return false;
        }
        std::println("[SYSTEM] World saved successfully.");
        return true;
    }
    else {
        // 8. Yazdırma başarısızsa false döndür
        std::println("[ERROR] World is not saved!");
        return false;
    }
}


bool GameManager::loadWorldState() {
    // Oda, enemy vb. verileri validate edene kadar enemy verilerini bir structta tutacağız
    struct SavedEnemy {
        int room_index;
        std::string name;
        double health;
    };
    // Ve bunları depolamak için bir vektör
    std::vector<SavedEnemy> savedEnemies;

    /*
    1. Önce dosya validate edilecek / geçici vektörlere okunacak.
    2. Her şey sağlamsa eski world silinip yeni world uygulanacak.
    */
    std::ifstream loadFile("worldsave.txt"); // Dosyayı okumak için aç

    if (loadFile.is_open()) {
        std::string version;
        int v_num;
        if (!(loadFile >> version >> v_num)) {
            return false;
        }
        if (version != "VERSION" || v_num != 1) {
            return false;
        }
        std::string n;
        int size;
        if (!(loadFile >> n >> size)) {
            return false;
        }
        if (n != "ENEMIES" || size != static_cast<int>(locations.size())) {
            return false;
        }


        for (int i = 0; i < size; i++) {
            int index;
            int amount;
            if (!(loadFile >> index >> amount)) {
                return false;
            }
            if (index < 0 || index >= size || amount < 0 || index != i) {
                std::println("Invalid index value!");
                return false;
            }
            for (int j = 0; j < amount; j++) {
                std::string name;
                double health;
                if (!(loadFile >> name >> health)) {
                    return false;
                }
                if (health < 0) {
                    return false;
                }
                SavedEnemy e1;
                e1.health = health;
                e1.name = name;
                e1.room_index = index;
                savedEnemies.push_back(e1);
            }
        }

        std::string tag;
        int num;
        if (!(loadFile >> tag >> num)) {
            return false;
        }

        if (tag != "ROOMS" || num != static_cast<int>(locations.size())) {
            return false;
        }

        // Saved itemlerin structı
        struct SavedItem {
            std::string name;
            double weight;
            double amount;
            int ammo;
            double damage;
            int accuracy;
            std::string item_type;
            int room_index;
        };

        // Ve onların vektörü
        std::vector<SavedItem> saved_items;

        for (int i = 0; i < num; i++) {
            int num1, num2;
            if (!(loadFile >> num1 >> num2)) {
                return false;
            }
            if (num1 < 0 || num1 >= num || num2 < 0 || num1 != i) {
                std::println("Invalid index value.");
                return false;
            }

            for (int j = 0; j < num2; j++) {
                std::string label;
                if (!(loadFile >> label)) { // Önce İLK kelimeyi (Etiketi) oku!
                    return false;
                }
                SavedItem i1{};
                i1.item_type = label;
                i1.room_index = num1;

                if (label == "WEAPON") {
                    std::string wName;
                    double wWeight, wDamage;
                    int wAmmo, wAccuracy;

                    if (!(loadFile >> wName >> wWeight >> wAmmo >> wDamage >> wAccuracy)) {
                        return false;
                    }
                    if (wAccuracy < 0 || wAccuracy > 100 || wWeight < 0 || wAmmo < 0 || wDamage < 0) {
                        return false;
                    }
                    i1.accuracy = wAccuracy;
                    i1.ammo = wAmmo;
                    i1.damage = wDamage;
                    i1.name = wName;
                    i1.weight = wWeight;
                }
                else if (label == "CONSUMABLE") {
                    std::string cName;
                    double cWeight, cAmount;

                    if (!(loadFile >> cName >> cWeight >> cAmount)) {
                        return false;
                    }
                    if (cWeight < 0 || cAmount < 0) {
                        return false;
                    }
                    i1.amount = cAmount;
                    i1.name = cName;
                    i1.weight = cWeight;

                }
                else if (label == "ITEM") {
                    std::string iName;
                    double iWeight;

                    if (!(loadFile >> iName >> iWeight)) {
                        return false;
                    }
                    if (iWeight < 0) {
                        return false;
                    }
                    i1.name = iName;
                    i1.weight = iWeight;
                }
                else {
                    return false;
                }
                saved_items.push_back(i1);
            }
        }

        // Visited odaların indekslerinin vektörü
        std::vector<int> savedVisited;

        // Visited rooms kısmı
        std::string vis;
        int vis_size;
        if (!(loadFile >> vis >> vis_size)) {
            return false;
        }
        if (vis != "VISITED" || vis_size < 0 || vis_size > static_cast<int>(locations.size())) {
            return false;
        }
        for (int i = 0; i < vis_size; i++) {
            int index;
            if (!(loadFile >> index)) {
                return false;
            }
            if (index < 0 || index >= static_cast<int>(locations.size())) {
                return false;
            }
            savedVisited.push_back(index);
        }


        // Objective kısmı
        std::string tagO;
        int stage_num;
        if (!(loadFile >> tagO)) {
            return false;
        }

        if (tagO != "OBJECTIVE_STAGE") {
            return false;
        }

        if (!(loadFile >> stage_num)) {
            return false;
        }
        if (stage_num < 0 || stage_num > 7) {
            return false;
        }

        // ARTIK COMMİT KISMI BAŞLIYOR PARSİNG BİTTİ
        clearAllRoomEnemies();
        clearAllRoomItems();
        visitedRooms.clear();

        // Enemyleri oluşturup odaya ekle
        for (int i = 0; i < static_cast<int>(savedEnemies.size()); i++) {
            auto temp = std::make_unique<Enemy>(savedEnemies[i].name, savedEnemies[i].health);
            locations[savedEnemies[i].room_index]->addEnemy(std::move(temp));
        }

        // İtemleri oluştur 
        for (int i = 0; i < static_cast<int>(saved_items.size()); i++) {
            if (saved_items[i].item_type == "WEAPON") {
                auto temp = std::make_unique<Weapon>(saved_items[i].name, saved_items[i].weight, saved_items[i].ammo, saved_items[i].damage, saved_items[i].accuracy);
                locations[saved_items[i].room_index]->addItem(std::move(temp));
            }
            else if (saved_items[i].item_type == "CONSUMABLE") {
                auto temp = std::make_unique<Consumable>(saved_items[i].name, saved_items[i].weight, saved_items[i].amount);
                locations[saved_items[i].room_index]->addItem(std::move(temp));
            }
            else if (saved_items[i].item_type == "ITEM") {
                auto temp = std::make_unique<Item>(saved_items[i].name, saved_items[i].weight);
                locations[saved_items[i].room_index]->addItem(std::move(temp));
            }
        }

        // Visited odalar kısmı
        for (int i = 0; i < static_cast<int>(savedVisited.size()); i++) {
            visitedRooms.push_back(locations[savedVisited[i]].get());
        }

        // Objective kısmı
        currentObjectiveStage = static_cast<ObjectiveStage>(stage_num);

        loadFile.close();
        std::println("[SYSTEM] World loaded successfully.");
        return true;
    }
    else {
        std::println("[ERROR] No world save file found! Starting with default world.");
        return false;
    }
}


void GameManager::startGame() {
	// Oyun döngüsü yeri
	while (gameContinue) {
        printStatus();      // HP - STAMINA ikilisini yazdır
        printMenu();        // Menü seçeneklerini yazdır
		
        int answer = readIntInRange(1, 9);
        
        // Tüm fonksiyonlar bir fonksiyon içinde 
        bool turnPassed = handlePlayerChoice(answer);
        
        // Düşman yaşıyorsa karşı atak yapar
        if (turnPassed && gameContinue) {
            chris.updateGame(); // Her turda karakterin durumuna göre staminası güncellenecek
            if (chris.getHealth() == 0) {
                std::println("\n========================");
                std::println("GAME OVER");
                std::println("That is the end of the road for Chris");
                std::println("========================");
                gameContinue = false;
                break;
            }
            handleEnemyCounterAttack();
        }
	}
}