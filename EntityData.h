#ifndef ENTITYDATA_H_INCLUDED
#define ENTITYDATA_H_INCLUDED

// mfb(t_flag) converts a flag to a bit for insertion into a bitfield
#ifndef mfb
#define mfb(n) long(1 << (n))
#endif

/**  Graphical data tree:

             area (areas)                       [layer: 0]
             turf (grass / floor / wall...)     [layer: 1]
    e_type - item (sword / shield...)           [layer: 2]
             mob (monsters / NPCs / Player)     [layer: 3] (player has a layer of 6)
             effect (effects)                   [layer: 4] (can be subject to change on behavior)

*/

/* ### ============== Entity data ============== ### */

// Root entity categories: (types of entities)
enum e_type {
    null = 0,
    turf,
    item,
    mob,
    area,
    effect,

    num_e_type // must be last; controls size of e_id
};


#endif // ENTITYDATA_H_INCLUDED
