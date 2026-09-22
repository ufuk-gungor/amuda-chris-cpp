# Amuda Chris

Amuda Chris is a console-based C++ action-adventure game developed as an object-oriented programming learning project.

The player controls Chris while exploring a small interconnected world, searching for clues and key items, managing limited resources, fighting enemies, and progressing toward a final encounter.

This is my first complete C++ OOP project and was built to turn the concepts I learned into a finished, playable program.

## Gameplay

The game is played through a numbered console menu.

The player can:

- Explore connected locations
- Search rooms for items
- Follow changing objectives
- Unlock areas using key items
- Fight enemies using different weapons
- Manage ammunition and inventory capacity
- Use healing consumables
- Sprint and climb using stamina
- Drop items and switch equipped weapons
- Save progress and continue later

The game ends after completing the main objective and returning to the required escape location.

## Features

### Exploration and Progression

- Multiple connected locations
- First-visit story events
- Locked areas
- Key-based progression
- Objective tracking
- Room inspection
- Story-driven item and enemy events

### Combat

- Turn-based combat
- Enemy counterattacks
- Weapon ammunition
- Weapon accuracy
- Randomized weapon damage
- Multiple weapons
- Boss encounter
- Game-over system

### Character Systems

- Health
- Stamina
- Sprinting
- Climbing
- Fall damage when climbing without enough stamina
- Inventory capacity based on item weight
- Healing consumables
- Equipped weapon management

### Inventory

The game contains different item types such as:

- Regular items
- Quest items
- Weapons
- Consumables

Quest items cannot be dropped, while regular items and weapons can be left in locations.

## Project Structure

The project is divided into several classes with separate responsibilities:

- `GameManager`  
  Controls the main game loop, world setup, objectives, combat flow, movement, searching, and world-state saving/loading.

- `MainCharacter`  
  Manages Chris's health, stamina, inventory, equipped weapon, movement state, and character save data.

- `Location`  
  Represents an area in the game world and stores its connections, items, enemies, and lock information.

- `Item`  
  Base class for inventory objects.

- `Weapon`  
  Derived from `Item`. Adds ammunition, damage, accuracy, firing behavior, and weapon-specific information.

- `Consumable`  
  Derived from `Item`. Represents healing items.

- `Enemy`  
  Stores enemy health and attack behavior.

- `StoryManager`  
  Handles story text, location introductions, item events, enemy encounters, boss dialogue, and the ending.

## C++ and OOP Concepts Used

This project was mainly developed to practice C++ and object-oriented programming.

Concepts used include:

- Classes and objects
- Encapsulation
- Inheritance
- Runtime polymorphism
- Virtual functions
- Function overriding
- `dynamic_cast`
- Composition
- Smart pointers
- `std::unique_ptr`
- Move semantics
- Non-owning raw pointers
- `std::vector`
- Enumerations
- Const-correctness
- File I/O
- Input validation
- Save-file validation
- Randomized combat values

### Ownership Model

The project uses `std::unique_ptr` for owned dynamically allocated objects.

For example:

- The game manager owns the locations.
- Locations own their items and enemies.
- The character owns the items in the inventory.

Raw pointers are used as non-owning references where ownership is handled elsewhere.

## Controls

The main menu contains the following actions:

1. Show character data
2. Attack an enemy
3. Use a healing item
4. Save and quit
5. Move to another location
6. Search the current room
7. Drop an item
8. Change equipped weapon
9. Inspect the current room

The current health, stamina, and objective are displayed during gameplay.

## Save System

The game stores character and world data separately.

Character save data includes:

- Player name
- Health
- Stamina
- Current location
- Inventory
- Item types
- Weapon ammunition
- Weapon statistics
- Equipped weapon

World save data includes:

- Remaining enemies
- Enemy health
- Items remaining in each location
- Visited locations
- Current objective

Save data is validated before being committed to the active game state.

Runtime save files are excluded from the Git repository through `.gitignore`.

## Building and Running

The project was developed with:

- C++
- Visual Studio 2022
- C++23

To build and run the project:

1. Clone or download the repository.
2. Open the Visual Studio solution.
3. Make sure the project is configured to use C++23.
4. Build the solution.
5. Run the application.

## Known Limitations

### Room Search and Dropped Items

Searching a room automatically attempts to collect all available items.

Because dropped items are placed back into the current location, dropping an unwanted item and immediately searching the same room may cause that item to be collected again.

Current workaround:

- Drop the unwanted item in another location.
- Return to the desired room.
- Search again.

A future version could allow the player to select individual items while searching.

### Save Files Are Not Fully Atomic

Character data and world data are stored in two separate files.

The game validates both save formats and contains recovery behavior, but the two files are not written as a single atomic transaction.

If one save file is missing or corrupted while the other remains valid, the game may recover using the valid character data together with a reconstructed/default world state.

A future version could store the entire game state in one unified save transaction.

## Possible Future Improvements

Potential improvements include:

- Individual item selection while searching rooms
- A unified save format
- More locations
- Additional enemy types
- More weapons and items
- Expanded combat mechanics
- Additional story content

## Project Purpose

The goal of this project was to build a complete program rather than isolated programming exercises.

It allowed me to practice how multiple systems interact inside one C++ application, including:

- Object ownership
- Class relationships
- Polymorphism
- Game-state management
- Input handling
- Inventory systems
- Combat systems
- Save/load functionality
- Error handling and validation

The project was inspired by cinematic action-adventure games and was created for educational purposes.

## Author

**Ufuk Talha Can Güngör**