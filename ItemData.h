#ifndef ITEMDATA_H_INCLUDED
#define ITEMDATA_H_INCLUDED
#include <string>
#include "libtcod.hpp"

/* Basic mass/volume notes:

    A typical hiking-grade backpack will carry about 70 volume units, and itself is 80 volume units.
    A typical apple is about 3 volume units and 0.3 kg.
    A typical handgun is about 5 volume units and 2 kg.
    A typical laser rifle is about 50 volume units and 13 kg.

    A typical human is about 1000 volume units and 70 kg.

*/


/* Item types */
enum i_type {
    i_null = 0,

    /*  Equipment  */
    i_equip,

        // Weapons:
        i_wep,
        i_dildo, // kek
        i_shortsword, i_longsword, i_staff,
        i_wep_end,
        // Clothes:
        i_cloth,
        i_sneakers, i_tshirt, i_jeans,
        i_backpack, i_satchel, i_purse, i_rucksack, // backpacks/bags/sacks
        i_cloth_end,
        // Armor:
        i_armor,
        i_leathervest, i_leatherleggings,
        i_armor_end,

    i_equip_end,

    /*  Food  */
    i_food,

        i_apple,

    i_food_end,

    num_i_type,
};

#endif // ITEMDATA_H_INCLUDED
