#pragma once
// Forward declaration: Pointer declaration için compiler’a “böyle bir class var” demek yeterli.
class Item;
class Enemy;
class Location;

class StoryManager {
public:
	void printEndingStory() const;
	void printStoryAfterPickup(Item*) const;
	void printStoryAfterEnemyKilled(Enemy*) const;
	void printLockedDoorStory(Location*) const;
	void printEnemyEncounterStory(Location*) const;
	void printFirstVisitStory(Location*) const;
};
