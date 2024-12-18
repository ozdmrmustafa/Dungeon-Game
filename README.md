# Dungeon Adventure Game

Dungeon Adventure Game is a text-based adventure game where the player explores a dungeon filled with rooms, items, and creatures. The player can move between rooms, interact with objects, and engage in battles with enemies.

---

## Gameplay

### Objective
Survive the dungeon by battling creatures, collecting powerful items, and navigating through interconnected rooms. Manage your inventory, plan your moves, and defeat all enemies to progress.

### Commands
The game accepts the following commands:

- **`move <R/L/U/D/B>`**: Move right (R), left (L), up (U), down (D), or back to the previous room (B).
- **`look`**: Observe the current room to see its description, items, and creatures.
- **`inventory`**: View your inventory and equipped items.
- **`pickup`**: Pick up an item in the room (if available).
- **`equip <1/2>`**: Equip an item from your inventory (slot 1 or 2).
- **`attack`**: Attack the creature in the room (if present).
- **`save <file>`**: Save the current game state to a file.
- **`load <file>`**: Load a previously saved game state from a file.
- **`exit`**: Exit the game.

### Example Session
1. Start in the entrance room with basic stats and no items equipped.
2. Move to adjacent rooms using commands like `move R`.
3. Encounter a creature and attack it using `attack`.
4. Collect items using `pickup` and manage your inventory with `equip`.
5. Save progress with `save` and reload it later with `load`.

---

## Code Structure

The game is divided into several modular components for clarity and reusability:

### 1. **Entities**
Defines the core structures used in the game:
- **`Item`**: Represents weapons and utility objects with attributes like `name`, `power`, `durability`, and whether it is a weapon.
- **`Creature`**: Represents enemies with attributes like `name`, `life`, `power`, and a `definition` that describes the creature.
- **`Room`**: Represents a dungeon room containing a description, an optional item, and an optional creature.

### 2. **Gameplay Logic**
Implements the main functionality for interacting with the game:
- **`moveCharacter`**: Allows the player to navigate between connected rooms.
- **`attack`**: Enables combat with creatures in the room.
- **`pickup`**: Allows the player to collect items.
- **`inventory`**: Displays the player's current inventory and equipped item.
- **`equipItem`**: Equips an item from the inventory.

### 3. **Persistence**
Handles saving and loading the game state:
- **`saveCommand`**: Logs player commands to a file for potential replay or restoration.
- **`loadGame`**: Reads commands from a file and replays them using `loadParseCommand`.

### 4. **User Interface**
Facilitates user interaction with the game:
- **`parseCommand`**: Processes player inputs and executes corresponding actions.
- **`look`**: Provides a detailed description of the current room.

### 5. **Memory Management**
Manages memory allocation and deallocation:
- **`freeRoom`**: Releases memory allocated for a room and its contents.
- **`resetLogFile`**: Clears the log file when a new game starts or the player dies.

---

## Game Logic

1. **Room Navigation**
   - Rooms are represented as a grid where the player can move in four cardinal directions or back to the previous room.
   - Each room can optionally contain an item and/or a creature.

2. **Combat System**
   - The player can attack creatures using their `power` attribute, enhanced by equipped items.
   - Creatures retaliate, reducing the player's life.
   - If the player's equipped item loses all durability, it breaks and must be replaced.

3. **Inventory Management**
   - The player has two inventory slots and one equipped item slot.
   - Items can be picked up from rooms and managed through inventory commands.

4. **Game State Management**
   - All commands are logged to a file (`logfile.txt`) for persistence.
   - The game state can be restored by replaying logged commands from the file.

