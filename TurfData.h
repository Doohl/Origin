#ifndef TURFDATA_H_INCLUDED
#define TURFDATA_H_INCLUDED

#define T_PARAMS   4


// Turf types:
enum t_type {
    t_null = 0,

    // Floors:
    t_grass, t_floor, t_wood_floor,

    // Dense:
    t_wall, t_window,
    t_water,

    num_t_type
};


// Turf property flags:
enum t_flag {
    t_transparent = 0,    // doesn't block vision
    t_walkable,           // doesn't block movement
    num_t_flag,
};


const int special_chars[4] = {
    // for grass:
    39, 96, 44, 46
};


#endif // TURFDATA_H_INCLUDED
