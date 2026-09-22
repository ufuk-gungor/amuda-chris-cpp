#include <string>
#include <print>
#include "StoryManager.h"
#include "Item.h"
#include "Enemy.h"
#include "Location.h"


void StoryManager::printEndingStory() const {
    std::println("Chris stumbles back into the Hidden Cave as the ruins shake behind him.");
    std::println("Dust pours from the ceiling. Somewhere in the distance, stone collapses with a deafening roar.");
    std::println("For a moment, Chris stops and looks back toward the darkness he came from.");
    std::println("The artifact in his bag gives off one last faint pulse.");
    std::println("Chris: 'All that trouble for one piece of history...'");
    std::println("His radio suddenly crackles.");
    std::println("Unknown Voice: 'Chris... if you can hear this, do not return home.'");
    std::println("Chris freezes.");
    std::println("Unknown Voice: 'Lazarevic was only the beginning. The shipment is already in London.'");
    std::println("The transmission cuts out.");
    std::println("Chris slowly lowers the radio.");
    std::println("Chris: 'London...'");
    std::println("A section of the cave collapses behind him, sealing the path back to the ruins.");
    std::println("Chris looks toward the daylight at the cave entrance and starts walking.");
    std::println("Chris: 'I really need a quieter hobby.'");
    std::println("==================================================");
    std::println("YOU ESCAPED THE RUINS.");
    std::println("LAZAREVIC IS DEAD.");
    std::println("THE ARTIFACT IS SAFE.");
    std::println("BUT SOMEONE ELSE IS ALREADY WAITING IN LONDON...");
    std::println("TO BE CONTINUED IN AMUDA CHRIS 2...");
    std::println("==================================================");
}


void StoryManager::printStoryAfterPickup(Item* item) const {
    if (item->getName() == "Silver_Key") {
        std::println("Chris turns the old silver key in his hand.");
        std::println("Fresh scratches cover its surface. Someone used it recently.");
        std::println("Chris: 'This does not belong in a cave... Lazarevic's men must have passed through here.'");
    }
    else if (item->getName() == "Gold_Key") {
        std::println("The gold key feels heavier than it should.");
        std::println("Ancient symbols are carved around its head, almost too precise to be handmade.");
        std::println("Chris: 'This has to open something important. Probably something I should not be opening.'");
    }
    else if (item->getName() == "Mercenary_Note") {
        std::println("Chris unfolds a dirt-stained note taken from the mercenary supplies.");
        std::println("Most of the writing is unreadable, but one sentence has been underlined twice:");
        std::println("\n'The gold key was moved to the western cliff path. Keep it away from the temple entrance.'");
        std::println("\nChris: 'Cliff Path. Finally, something resembling directions.'");
    }
    else if (item->getName() == "AK-47") {
        std::println("Chris picks up the rifle and checks the magazine.");
        std::println("This was not left behind by accident. The mercenaries came ready for a war.");
        std::println("Chris: 'Lazarevic is not just hunting treasure. He is preparing for trouble.'");
    }
    else if (item->getName() == "Heal") {
        std::println("Chris finds a small medical kit among the scattered supplies.");
        std::println("It is not much, but out here even a little help can keep him alive.");
        std::println("Chris: 'Not exactly hospital grade... but I will take it.'");
    }
    else if (item->getName() == "Strange_Vial") {
        std::println("Chris carefully lifts the strange vial.");
        std::println("The liquid inside moves slowly, dark and thick, like it is alive.");
        std::println("Chris: 'Yeah... definitely not drinking this unless things get very weird.'");
    }
    else if (item->getName() == "Ancient_Artifact") {
        std::println("The artifact pulses faintly in Chris's hand.");
        std::println("For a moment, the entire temple seems to breathe.");
        std::println("Chris: 'Okay... that is new.'");
    }
    else if (item->getName() == "First_Aid_Kit") {
        std::println("FIRST AID KIT!!! Just in time.");
    }
}


void StoryManager::printStoryAfterEnemyKilled(Enemy* enemy) const {
    if (enemy->getName() == "Mercenary") {
        std::println("The mercenary drops to the ground, his radio crackling beside him.");
        std::println("A broken voice comes through the static: 'Report... did you find the trail of artifact?'");
        std::println("Chris: 'The artifact's trail? Great. So this goes deeper than I thought.'");
    }
    else if (enemy->getName() == "Talbot") {
        std::println("Talbot staggers back, still smiling like he knows something Chris does not.");
        std::println("Talbot: 'You are chasing shadows, Chris. Lazarevic is already ahead of you.'");
        std::println("Chris: 'Yeah? Then I guess I better catch up.'");
    }
    else if (enemy->getName() == "Lazarevic") {
        std::println("Lazarevic falls hard against the temple floor.");
        std::println("For the first time, the confidence in his eyes disappears.");
        std::println("Lazarevic: 'You have no idea what you have found...'");
    }
}


void StoryManager::printLockedDoorStory(Location* destination) const {
    if (destination->getLock() && destination->getName() == "Mercenary Camp") {
        std::println("Chris examines the reinforced gate.");
        std::println("A small silver lock is hidden beneath the rusted metal plate.");
        std::println("Chris: 'Looks like I am missing the right key.'");
    }
    else if (destination->getLock() && destination->getName() == "Ancient Temple") {
        std::println("Chris presses against the enormous temple door, but it does not move.");
        std::println("A key-shaped indentation sits at the center of the ancient mechanism.");
        std::println("Chris: 'Let me guess... the expensive-looking key.'");
    }
}


void StoryManager::printEnemyEncounterStory(Location* destination) const {
    if (destination->getName() == "Jungle Ruins") {
        std::println("A rifle bolt clicks somewhere behind the broken pillars.");
        std::println("Mercenary: 'Do not move! Lazarevic wants the ruins sealed.'");
        std::println("A second figure steps out of the shadows, calmly adjusting his gloves.");
        std::println("Talbot: 'You are very far from home, Chris.'");
        std::println("Chris reaches for his weapon.");
        std::println("Chris: 'Funny. I was about to say the same thing.'");
    }
    else if (destination->getName() == "Ancient Temple") {
        std::println("Slow footsteps echo across the temple chamber.");
        std::println("Lazarevic stands beneath the ancient altar, the artifact glimmering in his hand.");
        std::println("Lazarevic: 'You followed the trail well, Chris. Better than my men expected.'");
        std::println("Chris: 'Your men should learn to hide their notes.'");
        std::println("Lazarevic closes his hand around the artifact.");
        std::println("Lazarevic: 'You came all this way just to die beside history.'");
        std::println("Chris draws his weapon.");
        std::println("Chris: 'You first.'");
    }
}


void StoryManager::printFirstVisitStory(Location* room) const {
    if (room == nullptr) {
        return;
    }

    std::string roomName = room->getName();

    std::println("\n--------------------------------------------------");

    if (roomName == "Hidden Cave") {
        std::println("Chris steps deeper into the Hidden Cave.");
        std::println("The air is cold, heavy, and strangely still.");
        std::println("Somewhere in the darkness, water drips like a slow countdown.");
        std::println("Chris: \"Great... dark cave, ancient ruins, probably something trying to kill me.\"");
    }
    else if (roomName == "Jungle Ruins") {
        std::println("Chris pushes through the thick jungle leaves.");
        std::println("Broken stone pillars rise from the mud, covered in moss and old symbols.");
        std::println("The place looks abandoned, but the fresh footprints say otherwise.");
        std::println("Chris: \"Someone got here before me. Of course they did.\"");
    }
    else if (roomName == "Mercenary Camp") {
        std::println("Chris enters the Mercenary Camp.");
        std::println("Torn maps, empty ammo crates, and dying campfires are scattered around.");
        std::println("This was not just a temporary stop. They were searching for something.");
        std::println("Chris: \"Lazarevic's men... and they came prepared.\"");
    }
    else if (roomName == "Cliff Path") {
        std::println("A narrow path curls around the side of the cliff.");
        std::println("The wind hits hard enough to make every step feel like a bad idea.");
        std::println("Far below, the jungle disappears into mist.");
        std::println("Chris: \"One wrong step and this adventure gets a very short ending.\"");
    }
    else if (roomName == "Ancient Temple") {
        std::println("Chris stands before the Ancient Temple.");
        std::println("Its golden walls are cracked, but still magnificent, as if time itself failed to bury it.");
        std::println("A low rumble echoes from inside. This is not just another ruin.");
        std::println("Chris: \"This is it. Whatever Lazarevic wanted... it's in there.\"");
    }
    else {
        std::println("Chris enters a new area.");
        std::println("The silence feels unnatural, like the place is waiting for him to make the first mistake.");
    }

    std::println("--------------------------------------------------\n");
}
