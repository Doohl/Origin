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

        /* Assigns all the values to this Item */
        void init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pmodifier, int pquantity, int pweight,
                       int pvolume, int pvalue, int prarity, int b_damage, int c_damage, int p_damage, int ppliancy, int pstack, int pmax_volume,
                       std::string pdesc, std::string pgroups);

        /* Initialize the item through text */
        void StrInit(std::string prototype);

        /* Add some extra description text */
        int GetDescAdd(TCODConsole* window, int lines);

        std::vector<Entity*> contents; // entities inside this object
        Entity* container; // the entity that is containing this item

        /* Weapon-specific members */
        virtual bool isWeapon() { return ((get_property<int>("blunt_damage") > 0) || (get_property<int>("cut_damage") > 0) || (get_property<int>("pierce_damage") > 0) || Helper::Find(groups, std::string("weapon"))); }

        /* Wearable-specific members */
        virtual bool isWearable() { return Helper::Find(groups, std::string("wearable")); }

        /* Comparison; returns true if item has the same variables as the other */
        bool operator== (const Item &comparison);

};

#endif // ITEM_H_INCLUDED
