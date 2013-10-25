#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED

#include "Entity.h"
#include "Helper.h"
#include <vector>
#include <iostream>
#include <map>

class Turf; // "Turf.h"

/*
    If a new variable is added or changed, be sure to initialize it in the constructor(s), initializers
    and copiers if appropriate.
*/

class Item : public Entity {
    public:
        Item();
        Item(std::string n, std::string d, char s, TCODColor c, int it, int mod, int quan, int w, int vol, int v, int r, int b_dam, int c_dam, int p_dam, int pli, int stak);
        ~Item();

        /* Initializes all starting / null values */
        virtual void init();

        /* Save the item's variables into a tokenized string */
        virtual std::string SaveEntity(Game* g);

        /* Reload the item's variables from a tokenized string */
        virtual void LoadEntity(std::string tokenized_data, Game* g);

        /* Copies all this item's relevant variables to another item */
        void CopyTo(Item* m);

        /* Assigns all the values to this Item */
        void init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pmodifier, int pquantity, int pweight,
                       int pvolume, int pvalue, int prarity, int b_damage, int c_damage, int p_damage, int ppliancy, int pstack, int pmax_volume,
                       std::string pdesc, std::string pgroups);

        /* Initialize the item through text */
        void StrInit(std::string prototype);

        /* Add some extra description text */
        int GetDescAdd(TCODConsole* window, int lines);

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

        // Wearable stats
        int blunt_defense;  // blunt damage defense
        int cut_defense;     // cut damage defense
        int pierce_defense;  // pierce damage defense

        int max_volume;              // maximum volume this item can contain (0 if not a container)
        std::vector<Entity*> contents; // entities inside this object

        int stack;      // how many items in a stack there are
        char index;     // character slot in the player's inventory
        Entity* container; // the entity that is containing this item

        /* Weapon-specific members */
        virtual bool isWeapon() { return (blunt_damage > 0 || cut_damage > 0 || pierce_damage > 0 || Helper::Find(groups, std::string("weapon"))); }

        /* Wearable-specific members */
        virtual bool isWearable() { return Helper::Find(groups, std::string("wearable")); }

        /* Comparison; returns true if item has the same variables as the other */
        bool operator== (const Item &comparison);

};

#endif // ITEM_H_INCLUDED
