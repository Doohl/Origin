#include "Item.h"
#include "Helper.h"
#include <algorithm>
#include <vector>
#include <iostream>

Item::Item() {
    init();
}

void Item::init() {
    etype = item;
    deleting = 0;
    name = "NULL";
    id = "NULL";
    desc = "NULL";
    layer = 2;
    modifier = 0;
    quantity = 0;
    weight = 0;
    volume = 0;
    value = 0;
    rarity = 0;
    blunt_damage = 0;
    cut_damage = 0;
    pierce_damage = 0;
    pliancy = 0;
    stack = 0;
    max_volume = 0;
    container = NULL;

}

void Item::init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pmodifier, int pquantity, int pweight,
                       int pvolume, int pvalue, int prarity, int b_damage, int c_damage, int p_damage, int ppliancy, int pstack, int pmax_volume,
                       std::string pdesc, std::string pgroups) {

    name = pname;
    id = pid;
    symbol = psymbol;
    color = pcolor;
    desc = pdesc;
    groups = Helper::Explode(';', pgroups);

    modifier = pmodifier;
    quantity = pquantity;
    weight = pweight;
    volume = pvolume;
    value = pvalue;
    rarity = prarity;

    blunt_damage = b_damage;
    cut_damage = c_damage;
    pierce_damage = p_damage;
    pliancy = ppliancy;

    stack = pstack;
    max_volume = pmax_volume;
}

Item::~Item() {
}

void Item::CopyTo(Item* i) {
    i->layer = layer;
    i->etype = etype;
    i->name = name;
    i->desc = desc;
    i->id = id;
    i->symbol = symbol;
    i->color = color;
    i->groups = groups;

    i->modifier = modifier;
    i->quantity = quantity;
    i->weight = weight;
    i->volume = volume;
    i->value = value;
    i->rarity = rarity;

    i->blunt_damage = blunt_damage;
    i->cut_damage = cut_damage;
    i->pierce_damage = pierce_damage;

    i->pliancy = pliancy;

    i->blunt_defense = blunt_defense;
    i->cut_defense = cut_defense;
    i->pierce_defense = pierce_defense;

    i->max_volume = max_volume;
}

int Item::GetDescAdd(TCODConsole* window, int lines) {
    if(!window) return lines;

    int returnlines = lines;

    window->setDefaultForeground(TCODColor(210, 210, 210));
    window->setDefaultBackground(TCODColor(114, 114, 114));
    window->setAlignment(TCOD_LEFT);

    if(isWearable()) { // clothing-specific statistics
        window->print(1, returnlines, "Maximum Volume: %i",  max_volume);
        returnlines++;
    }
    if(isWeapon()) {   // weapon-specific statistics
        window->print(1, returnlines, "Blunt: %i  |  Cut:  %i  |  Pierce: %i",  blunt_damage, cut_damage, pierce_damage);
        returnlines++;
        window->print(1, returnlines, "Pliancy Bonus: %i",  pliancy);
        returnlines++;
    }

    return returnlines;
}
