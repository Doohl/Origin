#include "Item.h"
#include "Helper.h"
#include <algorithm>
#include <vector>
#include <iostream>

Item::Item() {
    etype = item;
    name = "";
    id = "";
    desc = "";
    layer = 2;
}

Item::Item(std::string n, std::string d, char s, TCODColor c, int it, int mod, int quant, int w, int vol, int v, int r, int b_dam, int c_dam, int p_dam, int pli, int stak) {
    etype = item;
    name = n;
    symbol = s;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower); // transform n to lowercase, make that the id
    id = n;
    desc = d;
    color = c;
    item_type = it;
    modifier = mod;
    quantity = quant;
    weight = w;
    volume = vol;
    value = v;
    rarity = r;
    blunt_damage = b_dam;
    cut_damage = c_dam;
    pierce_damage = p_dam;
    blunt_defense = 0;
    cut_defense = 0;
    pierce_defense = 0;
    max_volume = 0;

    pliancy = pli;
    stack = stak;
    layer = 2;
}

Item::~Item() {
}

/* DEPRECATED
void Item::StrInit(std::string prototype) {


    Key:
        [name:desc:symbol:modifier:weight:value:rarity:bluntdamage:cutdamage:piercedamage]


    std::vector<std::string> parameters = Helper::Explode(':', prototype);
    name = parameters[0];
    desc = parameters[1];
    symbol = parameters[2][0]; // the first character in the symbol string (Ss) turns into (S), (C) = (C)
    modifier = Helper::str2int(parameters[3]);
    weight = Helper::str2int(parameters[4]);
    value = Helper::str2int(parameters[5]);
    rarity = Helper::str2int(parameters[6]);

    blunt_damage = Helper::str2int(parameters[7]);
    cut_damage = Helper::str2int(parameters[8]);
    pierce_damage = Helper::str2int(parameters[9]);

}
*/

void Item::CopyTo(Item* i) {
    i->layer = layer;
    i->etype = etype;
    i->name = name;
    i->desc = desc;
    i->id = id;
    i->symbol = symbol;
    i->color = color;
    i->item_type = item_type;
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

    if(isClothing()) { // clothing-specific statistics
        window->print(1, returnlines, "Maximum Volume: %i",  max_volume);
        returnlines++;
    }
    if(isWeapon()) {   // weapon-specific statistics
        window->print(1, returnlines, "Blunt: %i  |  Cut:  %i  |  Pierce: %i",  blunt_damage, cut_damage, pierce_damage);
        returnlines++;
    }
    if(isClothing() || isWeapon()) { // statistics applicable only to weapons and clothing
        window->print(1, returnlines, "Pliancy Bonus: %i",  pliancy);
        returnlines++;
    }

    return returnlines;
}

bool Item::operator==(const Item &comparison) {

    /* Compare variables */ // (Add identifying variables to this list)
    return (layer == comparison.layer && etype == comparison.etype && name == comparison.name && id == comparison.id &&
            symbol == comparison.symbol && color == comparison.color && item_type == comparison.item_type &&
            modifier == comparison.modifier && quantity == comparison.quantity && weight == comparison.weight &&
            volume == comparison.volume && value == comparison.value && rarity == comparison.rarity &&
            blunt_damage == comparison.blunt_damage && cut_damage == comparison.cut_damage && pierce_damage == comparison.pierce_damage &&
            blunt_defense == comparison.blunt_defense && cut_defense == comparison.cut_defense && pierce_defense == comparison.pierce_defense &&
            max_volume == comparison.max_volume && pliancy == comparison.pliancy);
}
