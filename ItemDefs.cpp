#include "Item.h"

Item itemlist[num_i_type];

/* Initializes a weapon or just a normal, mundane item */
void ITEM(std::string n, std::string d, char s, TCODColor c, int it, int rarity, int weight, int volume, int value,
            int blunt_damage, int cut_damage, int pierce_damage, int pliancy) {

    itemlist[it] = Item(n, d, s, c, it, 0, 0, weight, volume, value, rarity, blunt_damage, cut_damage, pierce_damage, pliancy, 1);
}

/* Initializes a clothing in itemlist */
void CLOTHING(std::string n, std::string d, char s, TCODColor c, int it, int rarity, int weight, int volume, int value,
            int max_volume, int blunt_defense, int cut_defense, int pierce_defense, int pliancy) {

    Item item = Item(n, d, s, c, it, 0, 0, weight, volume, value, rarity, 0, 0, 0, pliancy, 1);

    // Type-specific variables:
    item.blunt_defense = blunt_defense;
    item.cut_defense = cut_defense;
    item.pierce_defense = pierce_defense;
    item.max_volume = max_volume;

    itemlist[it] = item;
}

/* Called in Game.cpp:
        Initializes itemlist array; prototypes for preset objects
*/

void InitItems() {

    /**
        ITEM() synopsis:

        #name #desc #symbol
        #color #itemtype #rarity #weight #volume #value
        #bluntdam #cutdam #piercedam #pliancy
    */
    ITEM("short sword", "A small blade great for close-quarters melee.", '/',
           TCODColor::white, i_shortsword, 5, 10, 15, 50,
           0, 10, 0, 3);

    ITEM("10 inch dildo", "Jesus H Christ, what the HELL is this??", '/',
           TCODColor::darkerYellow, i_dildo, 20, 2, 5, 5,
           3, 0, 0, 0);

    /**
        CLOTHING() synopsis:

        #name #desc #symbol
        #color #itemtype #rarity #weight #volume #value
        #max_volume
        #blunt_defense #cut_defense #pierce_defense #pliancy
    */
    CLOTHING("t-shirt", "A simple t-shirt made of light fabric.", '[',
           TCODColor::darkGrey, i_tshirt, 1, 1, 3, 10,
           5,
           2, 0, 0, 0);

    CLOTHING("jeans", "Some simple blue exercise jeans.", ']',
           TCODColor::desaturatedBlue, i_jeans, 1, 2, 4, 10,
           10,
           2, 2, 1, 0);

    CLOTHING("backpack", "A basic container that goes on your back. It can be used to store a lot of things.", '[',
           TCODColor(102, 102, 51), i_backpack, 5, 4, 45, 100,
           40,
           0, 0, 0, 0);
}
