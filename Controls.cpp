#include "Game.h"
#include "Entity.h"
#include "Mob.h"
#include "Item.h"
#include "Map.h"
#include "Helper.h"
#include <vector>
#include <iostream>

void Game::HandleKeys(TCOD_key_t& key) {

    /* Inventory screen */
    if(player.viewinginventory) {

        // Space exits the menu:
        if(key.vk == TCODK_SPACE) {
            player.viewinginventory = false;
            player.selectingwear = false;
            player.selectingwield = false;
            player.selecteditem = NULL;
        } else {
            for(int i = 0; i < player.inventory.size(); i++) {
                if(player.inventory[i]->index == key.c) {

                    // We're expecting the player to wear the next item they select:
                    if(player.selectingwear) {
                        player.Wear(player.inventory[i]);
                        player.selectingwear = false;
                        player.viewinginventory = false;
                    }
                    // We're expecting the player to wield the next item they select:
                    else if(player.selectingwield) {
                        player.Wield(player.inventory[i]);
                        player.selectingwield = false;
                        player.viewinginventory = false;
                    }
                    // Default inventory: select an item to view description. Selecting an item again de-selects
                    else {
                        if(player.selecteditem == player.inventory[i]) {
                            player.selecteditem = NULL;
                        } else {
                            player.selecteditem = player.inventory[i];
                        }
                    }
                }
            }
        }
    }

    /* Default screen */
    else {

        // Movement keys:

        if(key.vk == TCODK_DOWN || key.vk == TCODK_KP2) {
            player.Step(0, 1, this);   // step down
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_UP || key.vk == TCODK_KP8) {
            player.Step(0, -1, this);  // step up
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_LEFT || key.vk == TCODK_KP4) {
            player.Step(-1, 0, this);  // step left
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_RIGHT || key.vk == TCODK_KP6) {
            player.Step(1, 0, this);   // step right
            ApplyTime(player.speed);

        } else if(key.vk == TCODK_KP1) {
            player.Step(-1, 1, this);  // step southwest
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_KP7) {
            player.Step(-1, -1, this);  // step northwest
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_KP9) {
            player.Step(1, -1, this);  // step northeast
            ApplyTime(player.speed);
        } else if(key.vk == TCODK_KP3) {
            player.Step(1, 1, this);  // step southeast
            ApplyTime(player.speed);

        // Action keys:

        } else if(key.c == '.') { // Wait a second
            ApplyTime(100);

        } else if(key.c == 'i') { // Access inventory
            player.viewinginventory = true; // toggle viewing inventory

        } else if(key.c == 'g') { // Pick up item
            Turf* t = player.turf;

            if(player.rhandselected) {
                if(player.righthand) {
                    return;
                }
            }else {
                if(player.lefthand) {
                    return;
                }
            }

            if(t->items.size() >= 1) {

                // Display a pickup message:
                std::string pickupmsg = "You pick up ";
                if(!Helper::proper(t->items[0]->name)) // if not a proper noun, apply correct grammar
                   pickupmsg += "the ";

                player.Message(pickupmsg + t->items[0]->name + ".", TCODColor::white, TCODColor::black);

                t->items[0]->container = &player;
                player.InventoryAdd(t->items[0]);

                if(player.rhandselected) {
                    player.righthand = t->items[0];
                    t->items[0]->index = 'a';
                }else {
                    player.lefthand = t->items[0];
                    t->items[0]->index = 'b';
                }


                t->RemoveContents(t->items[0], false);
                t->RemoveItems(t->items[0], false);
            }

        } else if(key.c == 'd') { // Drop item on hand
            Turf* t = player.turf;
            Item* dropitem = NULL;
            if(player.rhandselected && player.righthand != NULL) {
                dropitem = player.righthand;
                player.righthand = NULL; // clear hand
            } if(!player.rhandselected && player.lefthand != NULL) {
                dropitem = player.lefthand;
                player.lefthand = NULL; // clear hand
            }
            if(dropitem != NULL) {

                player.InventoryRemove(dropitem, false); // remove the item from the inventory (without deleting it)

                // Display drop message:
                std::string dropmsg = "You drop ";
                if(!Helper::proper(dropitem->name)) { // if not a proper noun, apply correct grammar
                    dropmsg += "the ";
                }
                player.Message(dropmsg + dropitem->name + ".", TCODColor::white, TCODColor::black);

                dropitem->container = t;
                t->contents.push_back(dropitem);
                t->items.push_back(dropitem);
                t->LayerContents();

            }

        } else if(key.c == 'W') { // Wear an item
            if(player.selectingwield)
                player.selectingwear = false;
            player.viewinginventory = true; // toggle viewing inventory
            player.selectingwear = true; // toggle selecting an item to wear
            player.selecteditem = NULL;

        } else if(key.c == 'w') { // wield an item
            if(player.selectingwear)
                player.selectingwear = false;
            player.viewinginventory = true; // toggle viewing inventory
            player.selectingwield = true; // toggle selecting an item to wear
            player.selecteditem = NULL;

        } else if(key.vk == TCODK_TAB) { // toggle the player's selected hand
            player.rhandselected = !player.rhandselected;

        // Debugging keys:
        } else if(key.vk == TCODK_F2) { // increase a_code
            debug_a_code++;
            std::cout << debug_a_code << std::endl;
        } else if(key.vk == TCODK_F1) { // decrease a_code
            debug_a_code--;
            std::cout << debug_a_code << std::endl;
        } else if(key.c == 'q') { // spawn goats
            std::vector<Turf*> SpawnTurfs = GameMap.TurfRange(player.x, player.y, 1);

            for(unsigned int i = 0; i < SpawnTurfs.size(); i++) {
                GameMap.SpawnMob(SpawnTurfs[i]->x, SpawnTurfs[i]->y, Mobs["m_goat"]);
            }
        } else if(key.c == 's') { // spawn skeletons
            std::vector<Turf*> SpawnTurfs = GameMap.TurfRange(player.x, player.y, 1);

            for(unsigned int i = 0; i < SpawnTurfs.size(); i++) {
                GameMap.SpawnMob(SpawnTurfs[i]->x, SpawnTurfs[i]->y, Mobs["m_skeleton"]);
            }

        }
    }
}
