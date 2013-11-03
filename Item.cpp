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
    symbol = ' ';
    c_symbol = 0;
    layer = 2;
    container = NULL;
    x = 0;
    y = 0;
    set_property("index", std::string("~"));
}

void Item::init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pmodifier, int pquantity, int pweight,
                       int pvolume, int pvalue, int prarity, int b_damage, int c_damage, int p_damage, int ppliancy, int pstack, int pmax_volume,
                       std::string pdesc, std::string pgroups) {

    symbol = psymbol;
    color = pcolor;
    groups = Helper::Explode(';', pgroups);

    set_property("name", pname);
    set_property("desc", pdesc);
    set_property("modifier", pmodifier);
    set_property("quantity", pquantity);
    set_property("weight", pweight);
    set_property("volume", pvolume);
    set_property("value", pvalue);
    set_property("rarity", prarity);
    set_property("blunt_damage", b_damage);
    set_property("cut_damage", c_damage);
    set_property("pierce_damage", p_damage);
    set_property("pliancy", ppliancy);
    set_property("stack", pstack);
    set_property("max_volume", pmax_volume);
}

Item::~Item() {
}

int Item::GetDescAdd(TCODConsole* window, int lines) {
    if(!window) return lines;

    int returnlines = lines;

    window->setDefaultForeground(TCODColor(210, 210, 210));
    window->setDefaultBackground(TCODColor(114, 114, 114));
    window->setAlignment(TCOD_LEFT);

    if(isWearable()) { // clothing-specific statistics
        window->print(1, returnlines, "Maximum Volume: %i",  get_property<int>("max_volume"));
        returnlines++;
    }
    if(isWeapon()) {   // weapon-specific statistics
        window->print(1, returnlines, "Blunt: %i  |  Cut:  %i  |  Pierce: %i",  get_property<int>("blunt_damage"), get_property<int>("cut_damage"), get_property<int>("pierce_damage"));
        returnlines++;
        window->print(1, returnlines, "Pliancy Bonus: %i",  get_property<int>("pliancy"));
        returnlines++;
    }

    return returnlines;
}
