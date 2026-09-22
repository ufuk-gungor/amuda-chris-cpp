#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Location.h"
#include "MainCharacter.h"
#include "StoryManager.h"

class GameManager {
public:
	GameManager();
	void startGame();

private:
	bool gameContinue;
	MainCharacter chris;
	StoryManager story_manager;
	Enemy* attacker;
	bool attacked = false;
	enum class ObjectiveStage {
		FindSilverKey = 0,
		ReachMercenaryCamp = 1,
		SearchCamp = 2,
		FindGoldKey = 3,
		UnlockTemple = 4,
		DefeatLazarevic = 5,
		RetrieveArtifact = 6,
		EscapeRuins = 7,
	};
	ObjectiveStage currentObjectiveStage;
	std::vector<Location*> visitedRooms;
	std::vector<std::unique_ptr<Location>> locations;
	Location* l1;
	Location* l2;
	Location* l3;
	Location* l4;
	Location* l5;

	Location* findLocationByName(const std::string&) const;

	bool isQuestItem(const std::string&) const;
	void move(Location*);
	bool canMoveTo(Location*) const;
	bool hasKeyFor(Location*) const;

	void printMenu() const;
	void printStatus() const;
	bool hasVisited(Location*) const;
	void markVisitedRoom(Location*);
	std::string getObjectiveText(ObjectiveStage) const;


	void handleEnemyCounterAttack();
	bool handleAttack();
	bool handleUsePotion();
	bool handleMovement();
	bool handleSearching();
	void handleDropItem();
	bool handleChangeWeapon();
	void handleInspectRoom() const;

	bool handlePlayerChoice(int);

	void setupLocations();
	void setupItemsAndLocks();
	void setupEnemies();
	void loadOrStartGame();
	void removeCollectedItemsFromRoom(Location*);
	void removeCollectedItemsFromWorld();

	bool saveWorldState() const;
	bool loadWorldState();
	void clearAllRoomItems();
	void clearAllRoomEnemies();

	int readIntInRange(int, int) const;

	void updateObjectiveAfterPickup(const std::string&);
	void updateObjectiveAfterMove(Location*);
	void printExitInfo(std::string, Location*) const;
	bool hasInventoryItem(const std::string&) const;
};