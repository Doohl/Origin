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
    symbol = ' ';
    c_symbol = 0;
    index = '~';
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
    x = 0;
    y = 0;

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

std::string Item::SaveEntity(Game* g) {
    std::string str;
    str += name + ";";
    str += id + ";";
    str += std::string(1, symbol) + ";";
    str += Helper::int2str(static_cast<int>(color.r)) + ";";
    str += Helper::int2str(static_cast<int>(color.g)) + ";";
    str += Helper::int2str(static_cast<int>(color.b)) + ";";
    str += desc + ";";
    str += Helper::Implode('&', groups) + ";";
    str += Helper::int2str(modifier) + ";";
    str += Helper::int2str(quantity) + ";";
    str += Helper::int2str(weight) + ";";
    str += Helper::int2str(volume) + ";";
    str += Helper::int2str(rarity) + ";";
    str += Helper::int2str(blunt_damage) + ";";
    str += Helper::int2str(cut_damage) + ";";
    str += Helper::int2str(pierce_damage) + ";";
    str += Helper::int2str(pliancy) + ";";
    str += Helper::int2str(stack) + ";";
    str += Helper::int2str(max_volume) + ";";
    str += std::string(1, index) + ";";
    return str;
}

void Item::LoadEntity(std::string tokenized_data, Game* g) {
    std::vector<std::string> data = Helper::Explode(';', tokenized_data);
    name = data[0];
    id = data[1];
    symbol = Helper::str2char(data[2]);
    color = TCODColor( Helper::str2int(data[3]), Helper::str2int(data[4]), Helper::str2int(data[5]));
    desc = data[6];
    groups = Helper::Explode('&', data[7]);
    modifier = Helper::str2int(data[8]);
    quantity = Helper::str2int(data[9]);
    weight = Helper::str2int(data[10]);
    volume = Helper::str2int(data[11]);
    rarity = Helper::str2int(data[12]);
    blunt_damage = Helper::str2int(data[13]);
    cut_damage = Helper::str2int(data[14]);
    pierce_damage = Helper::str2int(data[15]);
    pliancy = Helper::str2int(data[16]);
    stack = Helper::str2int(data[17]);
    max_volume = Helper::str2int(data[18]);
    index = Helper::str2char(data[19]);
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
