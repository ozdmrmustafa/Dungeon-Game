#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Item{
    char name[10];
    int power;
    int durability;
    bool isWeapon;
};

struct ItemNode{
    struct Item data;
    struct ItemNode *next;
    struct ItemNode *prev;
};


struct Item createItem(char name[],int power,int durability,bool isWeapon){
    struct Item newItem;
    strncpy(newItem.name, name, sizeof(newItem.name) - 1);
    newItem.name[sizeof(newItem.name) - 1] = '\0'; // Güvenlik için null-terminate
    newItem.power = power;
    newItem.durability = durability;
    newItem.isWeapon=isWeapon;

    return newItem;
};


struct Creature {
    int life;
    int power;
    char name[10];
    char definition[100];
};

//yaratık oluşturma
struct Creature *createCreature(int life, int power, char name[10], char definition[100]) {
    // Yeni bir Creature için bellek tahsisi
    struct Creature *newCreature = (struct Creature *)malloc(sizeof(struct Creature));
    if (newCreature == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Alanların doldurulması
    newCreature->life = life;
    newCreature->power = power;

    // Dizileri kopyala (dizilere doğrudan atama yapılamaz)
    strncpy(newCreature->name, name, sizeof(newCreature->name) - 1);
    newCreature->name[sizeof(newCreature->name) - 1] = '\0'; // Null-terminate

    strncpy(newCreature->definition, definition, sizeof(newCreature->definition) - 1);
    newCreature->definition[sizeof(newCreature->definition) - 1] = '\0'; // Null-terminate

    return newCreature;
}

//açıklaması, item leri ve yaratıkları ile bir oda nın içieriği ile alakalı kısım
struct Room{
    char definition[100];
    struct Item *item;
    struct Creature *creature;
    char description[100];
};

//oda yı bir node olarak oluşturduk data sı room
struct RoomNode{
    struct Room data;
    struct RoomNode *up;
    struct RoomNode *down;
    struct RoomNode *right;
    struct RoomNode *left;
};

//oda yı oluşturma
struct RoomNode *createRoom(char definition[100], struct Item *item, struct Creature *creature) {
    struct RoomNode *newRoom = (struct RoomNode *)malloc(sizeof(struct RoomNode));
    if (newRoom == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Room verilerini kopyala
    strncpy(newRoom->data.definition, definition, sizeof(newRoom->data.definition) - 1);
    newRoom->data.definition[sizeof(newRoom->data.definition) - 1] = '\0';

    // Item için kontrol ve bellek tahsisi
    if (item != NULL) {
        newRoom->data.item = (struct Item *)malloc(sizeof(struct Item));
        if (newRoom->data.item == NULL) {
            printf("Memory allocation failed for item!\n");
            exit(1);
        }
        memcpy(newRoom->data.item, item, sizeof(struct Item));
    } else {
        newRoom->data.item = NULL;
    }

    // Creature için kontrol ve bellek tahsisi
    if (creature != NULL) {
        newRoom->data.creature = (struct Creature *)malloc(sizeof(struct Creature));
        if (newRoom->data.creature == NULL) {
            printf("Memory allocation failed for creature!\n");
            exit(1);
        }
        memcpy(newRoom->data.creature, creature, sizeof(struct Creature));
    } else {
        newRoom->data.creature = NULL;
    }

    // Pointer'ları NULL olarak başlat
    newRoom->up = NULL;
    newRoom->down = NULL;
    newRoom->right = NULL;
    newRoom->left = NULL;

    return newRoom;
}

//bir oda ile diğerini bağlamak içi
void addRoom(struct RoomNode *room1, struct RoomNode *room2, char direction) {
    if (direction == 'R') { // room2, room1'in sağına eklenir
        room1->right = room2;
        room2->left = room1;
    } else if (direction == 'L') { // room2, room1'in soluna eklenir
        room1->left = room2;
        room2->right = room1;
    } else if (direction == 'U') { // room2, room1'in üstüne eklenir
        room1->up = room2;
        room2->down = room1;
    } else if (direction == 'D') { // room2, room1'in altına eklenir
        room1->down = room2;
        room2->up = room1;
    } else {
        printf("Invalid direction! Use R (right), L (left), U (up), or D (down).\n");
    }
}


struct Character{
    char name[15];
    int life;
    int power;
    struct Item *item[2];       // Envanter kapasitesi 2 item
    struct Item *equippedItem;  // Elinizde tuttuğunuz silah
    struct RoomNode *currentDirection;
    struct RoomNode *firstDirection;
};


void moveCharacter(struct Character *character, char direction) {
    struct RoomNode *currentRoom = character->currentDirection;
    struct RoomNode *nextRoom = NULL;

    if (direction == 'R') {
        nextRoom = currentRoom->right;
    } else if (direction == 'L') {
        nextRoom = currentRoom->left;
    } else if (direction == 'U') {
        nextRoom = currentRoom->up;
    } else if (direction == 'D') {
        nextRoom = currentRoom->down;
    } else if (direction == 'B') { // Back komutu
        if (character->firstDirection != NULL) {
            nextRoom = character->firstDirection;
        } else {
            printf("No previous room to go back to.\n");
            return;
        }
    } else {
        printf("Invalid direction! Use R (right), L (left), U (up), D (down), or B (back).\n");
        return;
    }

    if (nextRoom != NULL) {
        character->firstDirection = character->currentDirection; // Önceki odayı kaydet
        character->currentDirection = nextRoom;                 // Yeni odaya geç
        printf("%s moves to the next room: %s\n", character->name, nextRoom->data.definition);
    } else {
        printf("There is no room in that direction.\n");
    }
}

struct Character *createCharacter(int life, int power, char name[10], struct Item *items[3], struct RoomNode *firstDirection) {
    struct Character *newCharacter = (struct Character *)malloc(sizeof(struct Character));
    if (newCharacter == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newCharacter->life = life;
    newCharacter->power = power;
    strncpy(newCharacter->name, name, sizeof(newCharacter->name) - 1);
    newCharacter->name[sizeof(newCharacter->name) - 1] = '\0';

    for (int i = 0; i < 2; i++) {
        newCharacter->item[i] = items[i];
    }

    newCharacter->currentDirection = firstDirection;
    return newCharacter;
}
void look(struct RoomNode *room) {
    printf("Room Description: %s\n", room->data.definition);
    if (room->data.item) {
        printf("Item: %s\n", room->data.item->name);
    }
    if (room->data.creature) {
        printf("Creature: %s\n", room->data.creature->name);
    }
}
bool pickup(struct Character *player, struct Item *item) {
    // Öncelikle eliniz boşsa silahı doğrudan alın
    if (player->equippedItem == NULL) {
        player->equippedItem = item;
        player->power+=player->equippedItem->power;
        printf("You equipped the %s.\n", item->name);
        return true;
    }

    // Envanter kapasitesini kontrol et
    for (int i = 0; i < 2; i++) {
        if (player->item[i] == NULL) {
            player->item[i] = item;
            printf("You picked up the %s.\n", item->name);
            return true;
        }
    }

    printf("Your inventory is full! Drop something to pick up %s.\n", item->name);
    return false;
}

void inventory(struct Character *player) {
    printf("Inventory:\n");
    for (int i = 0; i < 2; i++) {
        if (player->item[i] != NULL) {
            printf("%d. %s (Power: %d, Durability: %d)\n", i + 1, player->item[i]->name, player->item[i]->power, player->item[i]->durability);
        } else {
            printf("%d. [Empty]\n", i + 1);
        }
    }
}
void attack(struct Character *player, struct Creature *creature) {
    if (creature == NULL || creature->life <= 0) {
        printf("No creature to attack here.\n");
        return;
    }

    /*if (player->equippedItem == NULL) {
        printf("You have no weapon equipped to attack!\n");
        return;
    }
*/
    printf("Attacking %s with %s...\n", creature->name, player->equippedItem->name);

    while (player->life > 0 && creature->life > 0) {
        // Silahın gücünü kullanarak saldır
        creature->life -= player->power;
        if (player->equippedItem!=NULL){
            player->equippedItem->durability -= 10; // Dayanıklılık azalır
        }

        printf("You hit %s. Creature's life: %d\n", creature->name, creature->life);

        if (player->equippedItem!=NULL && player->equippedItem->durability <= 0) {
            printf("Your %s broke and is no longer usable!\n", player->equippedItem->name);
            player->equippedItem = NULL; // Silahı elden çıkar
            break;
        }

        if (creature->life > 0) {
            player->life -= creature->power;
            printf("%s hits back. Your life: %d\n", creature->name, player->life);
        }
    }

    if (player->life <= 0) printf("You died!\n");
    if (creature->life <= 0){
        printf("You defeated %s!\n", creature->name);
        player->currentDirection->data.creature=NULL;
    }

}
void equipItem(struct Character *player, int itemIndex) {
    if (itemIndex < 0 || itemIndex >= 2 || player->item[itemIndex] == NULL) {
        printf("Invalid item selection!\n");
        return;
    }

    struct Item *newItem = player->item[itemIndex];
    printf("Equipping %s...\n", newItem->name);

    // Eğer elinizde bir silah varsa, onu envanterdeki mevcut item ile takas edin
    if (player->equippedItem != NULL) {
        printf("Swapping %s with %s...\n", player->equippedItem->name, newItem->name);
        player->item[itemIndex] = player->equippedItem; // Eski silah envantere geri gider
        player->power -= player->equippedItem->power;   // Eski silah gücü çıkarılır
    } else {
        printf("Equipping new item: %s.\n", newItem->name);
        player->item[itemIndex] = NULL; // Seçilen item slotunu boşalt
    }

    player->equippedItem = newItem; // Yeni silahı elinize alın
    player->power += newItem->power; // Yeni silah gücü eklenir
    printf("You equipped: %s (Power: %d)\n", newItem->name, newItem->power);
}


void saveCommand(const char *command, const char *logFilePath) {
    FILE *file = fopen(logFilePath, "a");
    if (!file) {
        printf("Failed to open log file: %s\n", logFilePath);
        return;
    }
    fprintf(file, "%s\n", command);
    fclose(file);
}

void loadParseCommand(char *command, struct Character *player, struct RoomNode *rooms[], int roomCount) {
    if (strncmp(command, "move ", 5) == 0) {
        char direction = command[5];
        moveCharacter(player, direction);
    } else if (strcmp(command, "back") == 0) {
        moveCharacter(player, 'B');
    } else if (strcmp(command, "look") == 0) {
        look(player->currentDirection);
    } else if (strcmp(command, "inventory") == 0) {
        inventory(player);
    } else if (strncmp(command, "pickup ", 7) == 0) {
        if (player->currentDirection->data.item != NULL) {
            pickup(player, player->currentDirection->data.item);
            player->currentDirection->data.item = NULL;
        }
    } else if (strncmp(command, "equip ", 6) == 0) {
        int index;
        sscanf(command, "equip %d", &index);
        equipItem(player, index - 1);
    } else if (strcmp(command, "attack") == 0) {
        if (player->currentDirection->data.creature) {
            attack(player, player->currentDirection->data.creature);
        }
    } else {
        printf("");
    }
}

void loadGame(struct Character *player, struct RoomNode *rooms[], int *roomCount, const char *logFilePath) {
    FILE *file = fopen(logFilePath, "r");
    if (!file) {
        printf("Failed to open log file: %s\n", logFilePath);
        return;
    }

    char command[50];
    while (fgets(command, sizeof(command), file)) {
        // Komutun sonunda olası '\n' karakterini temizle
        command[strcspn(command, "\n")] = 0;
        loadParseCommand(command, player, rooms, *roomCount);
    }
    fclose(file);
    printf("Game loaded from %s\n", logFilePath);
}



void parseCommand(char *command, struct Character *player, struct RoomNode *rooms[], int roomCount) {
    saveCommand(command, "logfile.txt"); // Komutu log dosyasına kaydet

    if (strncmp(command, "move ", 5) == 0) {
        char direction = command[5];
        moveCharacter(player, direction);
        printf("You moved to a new room.\n");
    } else if (strcmp(command, "back") == 0) {
        moveCharacter(player, 'B');
        printf("You moved back to the previous room.\n");
    } else if (strcmp(command, "look") == 0) {
        printf("You look around the room:\n");
        look(player->currentDirection);
    } else if (strcmp(command, "inventory") == 0) {
        printf("Checking your inventory...\n");
        inventory(player);
    } else if (strncmp(command, "pickup ", 7) == 0) {
        if (player->currentDirection->data.item != NULL) {
            printf("You attempt to pick up the %s...\n", player->currentDirection->data.item->name);
            pickup(player, player->currentDirection->data.item);
            player->currentDirection->data.item = NULL;
        } else {
            printf("No item to pick up in this room.\n");
        }
    } else if (strncmp(command, "equip ", 6) == 0) {
        int index;
        sscanf(command, "equip %d", &index);
        printf("Equipping item %d...\n", index);
        equipItem(player, index - 1);
    } else if (strcmp(command, "attack") == 0) {
        if (player->currentDirection->data.creature) {
            printf("You prepare to attack the %s!\n", player->currentDirection->data.creature->name);
            attack(player, player->currentDirection->data.creature);
        } else {
            printf("No creature to attack here!\n");
        }
    } else if (strncmp(command, "save ", 5) == 0) {
        char filepath[50];
        sscanf(command, "save %s", filepath);
        printf("Saving game to file: %s...\n", filepath);
    } else if (strncmp(command, "load ", 5) == 0) {
        char filepath[50];
        sscanf(command, "load %s", filepath);
        printf("Loading game from file: %s...\n", filepath);
        loadGame(player, rooms, &roomCount, filepath);
    } else if (strcmp(command, "help") == 0) {
        printf("\nAvailable Commands:\n");
        printf("move <R/L/U/D>   - Move in a direction (Right/Left/Up/Down)\n");
        printf("back             - Return to the previous room\n");
        printf("look             - Look around the current room\n");
        printf("inventory        - Show your inventory and equipped item\n");
        printf("pickup <item>    - Pick up an item in the room\n");
        printf("equip <1/2>      - Equip an item from your inventory\n");
        printf("attack           - Attack the creature in the room\n");
        printf("save <file>      - Save the current game state\n");
        printf("load <file>      - Load a saved game state\n");
        printf("exit             - Exit the game\n");
    } else if (strcmp(command, "exit") == 0) {
        printf("Exiting the game. Goodbye!\n");
        exit(0);
    } else {
        printf("Invalid command! Type 'help' to see a list of available commands.\n");
    }

    printf("\n\nType: move <R/L/U/D/B>, back, look, inventory, pickup <item>,equip <1/2>, attack, save <file>, load <file>, help, exit\n");
    printf("❤: %d  💪🏻: %d\n", player->life, player->power);
}

void resetLogFile(const char *logFilePath) {
    FILE *file = fopen(logFilePath, "w");
    if (!file) {
        printf("Failed to reset log file: %s\n", logFilePath);
        return;
    }
    fclose(file);
}

void freeRoom(struct RoomNode *room) {
    if (room->data.item) free(room->data.item);
    if (room->data.creature) free(room->data.creature);
    free(room);
}


int main(){
    // Items
    struct Item sword = createItem("Sword", 30, 40, true);
    struct Item shield = createItem("Baton", 10, 30, false);
    struct Item gun = createItem("Gun", 50, 70, true);

    //Creatures
    struct Creature *dragon = createCreature(100, 30, "Dragon", "A fire-breathing mythical creature.");
    struct Creature *goblin = createCreature(50, 20, "Goblin", "A sneaky little monster.");
    struct Creature *mosquito = createCreature(100, 70, "Mosquito", "A tiny and annoying insect, buzzing around incessantly.");



    //Rooms
    struct RoomNode *room1 = createRoom("Entrance Room", &sword, NULL);
    struct RoomNode *room2 = createRoom("Dragon's Den", NULL, dragon);
    struct RoomNode *room3 = createRoom("Treasure Room", &gun, NULL);
    struct RoomNode *room4 = createRoom("Goblin Cave", &shield, goblin);
    struct RoomNode *room5 = createRoom("Buzzing room", &sword, mosquito);

    struct RoomNode *rooms[4] = {room1, room2, room3, room4};
    int roomCount = 4;


    // creating map
    addRoom(room1, room2, 'R');
    addRoom(room1, room3, 'U');
    addRoom(room2, room4, 'U');
    addRoom(room4,room5,'U');

    // creating player
    struct Item *items[3] = {NULL, NULL, NULL};
    struct Character *player = createCharacter(100, 50, "Hero", items, room1);

    //setting current direction of player
    player->currentDirection = room1;

    //game is starting
    char command[50];
    printf("Welcome to the Dungeon Adventure Game!\n");
    printf("Type: move <R/L/U/D/B>, back, look, inventory, pickup <item>,equip <1/2>, use <index>, attack, save <file>, load <file>, help, exit\n");
    printf("❤\uFE0F:%d  💪🏻:%d",player->life,player->power);


    while (1) {
        printf("\nEnter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Enter tuşunu kaldır

        // Komutları kaydet
        saveCommand(command, "logfile.txt");

        if (strncmp(command, "move ", 5) == 0) {
            char direction = command[5];
            moveCharacter(player, direction);
        } else if (strcmp(command, "exit") == 0) {
            printf("Exiting the game. Goodbye!\n");
            break;
        } else if (strncmp(command, "load ", 5) == 0) {
            char filepath[50];
            sscanf(command, "load %s", filepath);
            loadGame(player, rooms, &roomCount, filepath);
        } else {
            parseCommand(command, player, rooms, roomCount);
        }

        // Oyuncu ölürse oyunu bitir
        if (player->life <= 0) {
            printf("Game Over!\n");
            resetLogFile("logfile.txt");
            break;
        }
    }


    // Bellek temizleme
    free(dragon);
    free(goblin);
    free(player);
    for (int i = 0; i < roomCount; i++) {
        freeRoom(rooms[i]);
    }


    return 0;
}
