#pragma once
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "Enemy.h"
#include "Item.h"

class Location {
public:
	Location(std::string, std::string);
	void printLocationInfo() const;
	void setConnections(Location*, Location*, Location*, Location*);
	Location* getNorth() const { return north; }
	Location* getSouth() const { return south; }
	Location* getEast() const { return east; }
	Location* getWest() const { return west; }
	std::string getName() const { return name; }
	std::string getDescription() const { return description; }
	void addEnemy(std::unique_ptr<Enemy>&& e) { enemies.push_back(std::move(e)); }
	const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
	std::vector<std::unique_ptr<Enemy>>& getEnemies() { return enemies; }
	void addItem(std::unique_ptr<Item>&& i) { items.push_back(std::move(i)); }
	const std::vector<std::unique_ptr<Item>>& getItems() const { return items; }
	std::vector<std::unique_ptr<Item>>& getItems() { return items; }
	void setKey(std::string);
	bool getLock() const { return isLocked; }
	std::string getKey() const { return key; }

private:
	std::string name;
	std::string description;
	Location* north;
	Location* south;
	Location* west;
	Location* east;
	std::vector<std::unique_ptr<Enemy>> enemies;
	std::vector<std::unique_ptr<Item>> items;
	bool isLocked;
	std::string key;
};