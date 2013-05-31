#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Mob.h"
#include "Entity.h"
#include "Item.h"
#include <vector>

// Message class: stored in Player::msgbuff
class Msg {
    public:
        Msg();
        ~Msg();

        std::string message;
        TCODColor fcolor;
        TCODColor bgcolor;
};

class Player : public Mob {

    public:
        Player();
        ~Player();

        /* Code executed after the player performs an action **/
        void DoLogic(Game* game);

        /* Attack a mob */
        void Attack(Mob* m, Game* game);

        /* Displays a message to the player */
        void Message(std::string txt, TCODColor fcolor, TCODColor bcolor);

        /* Polymorphic member from parent Mob; updates location and camera */
        void Move(int newx, int newy);

        /* Polymorphic member from parent Mob; steps in a direction, optionally attacks something */
        void Step(int stepx, int stepy, Game* g);

        /* Find the next available inventory character */
        char InvNextChar();

        /* Add an item to the inventory */
        void InventoryAdd(Item* item);

        /* Re-sorts inventory indexes */
        void ResortInventory();

        /* Removes an item from the inventory vector */
        void InventoryRemove(Item* item, bool dodel);

        /* Removes an item from the Worn vector */
        void WornRemove(Item* item, bool dodel);

        /* Wear an item on the player */
        void Wear(Item* item);

        /* Wield an item in the player's inventory to selected hand */
        void Wield(Item* item);

        Game* game;

        bool cam_follow;        // true if camera is centered on the player whenever he moves
        int cam_x;              // location of the player's camera
        int cam_y;              // location of the player's camera

        std::vector<Item*> inventory; // player's inventory (includes all items)
        bool viewinginventory;  // if the player is looking at the inventory
        bool selectingwear;     // if the next item the player selects in the inventory will be worn
        bool selectingwield;
        int inventorypage;      // current page in the inventory
        Item* selecteditem;     // selected item / viewing item

        Item* lefthand;         // item the player is holding in the left hand
        Item* righthand;        // item the player is holding in the right hand
        bool rhandselected;     // if true, right hand selected. else, left hand

        std::vector<Item*> Worn;// items the player is wearing

        std::vector<Msg*> msgbuff; // buffer containing messages

        /* Player stats: */
        float speed;            // How much time passes by per movement; normally 100.0f

};

#endif // PLAYER_H_INCLUDED
