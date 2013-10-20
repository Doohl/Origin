#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Entity.h"
#include "ItemData.h"
#include <vector>
#include <map>

class Turf; // "Turf.h"

/*
    IMPORTANT! Please read:

    If a new variable is added or changed, be sure to initialize it in the constructor(s) if appropriate.

    New variables specific to a certain type of item should be set to its default in a constructor but not
    actually modified in it. Type-specific variables should instead be modified under ItemDefs.cpp.

    Be sure to update the == overloader and Item::CopyTo(), or things will break.
*/

class Item : public Entity {
    public:
        Item();
        Item(std::string n, std::string d, char s, TCODColor c, int it, int mod, int quan, int w, int vol, int v, int r, int b_dam, int c_dam, int p_dam, int pli, int stak);
        ~Item();

        /* Initializes all starting / null values */
        virtual void init();

        /* Assigns all the values to this Item */
        void init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pmodifier, int pquantity, int pweight,
                       int pvolume, int pvalue, int prarity, int b_damage, int c_damage, int p_damage, int ppliancy, int pstack, int pmax_volume,
                       std::string pdesc, std::string pgroups);

        /* Copy all members to another Item pointer */
        void CopyTo(Item* i);

        /* Initialize the item through text */
        void StrInit(std::string prototype);

        /* Add some extra description text */
        int GetDescAdd(TCODConsole* window, int lines);

        int item_type;  // i_type enum value for this item (ie, i_wep_shortsword)
        int modifier;   // can be hunger modifier, effect modifier, etc
        int quantity;   // how many uses are left. For shit like food and medicine, rechargeables, etc.
        int weight;     // the weight of this item (in kilograms)
        int volume;     // the volume of this item, how much space it takes up
        int value;      // arbitrary number for how valueable this item is
        int rarity;     // higher number = rarer

        // Weapon stats
        int blunt_damage;   // blunt damage (ie pipes, hammers, improvised weapons)
        int cut_damage;     // cutting damage (ie swords)
        int pierce_damage;  // piercing damage (ie spears)


        int pliancy;        // adds to your hit chance (if negative, removes)
                            // if applied to clothing, determines speed boost or drain

        // Clothing stats
        int blunt_defense;  // blunt damage defense
        int cut_defense;     // cut damage defense
        int pierce_defense;  // pierce damage defense

        int max_volume;              // maximum volume this item can contain (0 if not a container)
        std::vector<Entity*> contents; // entities inside this object

        int stack;      // how many items in a stack there are
        char index;     // character slot in the player's inventory
        Entity* container; // the entity that is containing this item

        /* Equipment-specific membes */
        // If the item is an equipment type
        virtual bool isEquip()  { return (item_type >= i_equip && item_type <= i_equip_end); }

        /* Weapon-specific members */
        // If the item is a weapon type
        virtual bool isWeapon() { return (item_type >= i_wep && item_type <= i_wep_end); }

        /* Clothing-specific members */
        // If the item is a clothing type
        virtual bool isClothing() { return (item_type >= i_cloth && item_type <= i_cloth_end);}

        /* Armor-specific members */
        // If the item is an armor type
        virtual bool isArmor() { return (item_type >= i_armor && item_type <= i_armor_end); }

        /* Consumeable-specific members (also drinks) */
        // If the item is a food type (drinks too I guess)
        virtual bool isFood() { return (item_type >= i_food && item_type <= i_food_end); }


        /* Comparison; returns true if item has the same variables as the other */
        bool operator== (const Item &comparison);

};

extern Item itemlist[num_i_type];
extern std::map< std::string, Item > _globalitems;

void InitItems();


#endif // ITEM_H_INCLUDED
