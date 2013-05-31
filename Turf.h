#ifndef TURF_H_INCLUDED
#define TURF_H_INCLUDED

#include "Entity.h"
#include "TurfData.h"
#include "Mob.h"
#include "Item.h"
#include <vector>

class Map;

/*
    IMPORTANT! Please read:

    If a new variable is added or changed, be sure to initialize it in the constructor(s) if appropriate.
    Be sure to update the == overloader and Turf::Copy_To(), or things will break.
*/

class Turf : public Entity {
    public:
        Turf();
        Turf(std::string n, unsigned int f, char s, TCODColor c);
        ~Turf();

        /* Copies the turf's properties onto another turf. Probably should revise. */
        void Copy_To(Turf& t);

        /* Removes an entity from the contents vector */
        void RemoveContents(Entity* remove, bool dodel);

        /* Removes a mob from the mobs vector */
        void RemoveMobs(Mob* remove, bool dodel);

        /* Removes an item from the items vector */
        void RemoveItems(Item* remove, bool dodel);

        /* Sort contents vector in relation to entities' layer */
        void LayerContents();

        std::vector<Entity*> contents;  // vector of game Entities "inside" the turf
        std::vector<Mob*> mobs;         // vector of game Mobs inside contents; subgroup
        std::vector<Item*> items;       // vector of game Items inside contents; subgroup
        Map* map;                       // address of map this turf is in

        int lastscreenx;
        int lastscreeny;

};


/* Array of prototyped turfs */
const Turf turflist[num_t_type] = {
    Turf("nothing",     mfb(t_transparent), ' ', TCODColor::black),       // t_null
    Turf("grass",       mfb(t_transparent)|mfb(t_walkable), '.',          // t_grass
         TCODColor(38, 102, 0)),
    Turf("floor",       mfb(t_transparent)|mfb(t_walkable), '+',          // t_floor
         TCODColor::grey),
    Turf("wood_floor",  mfb(t_transparent)|mfb(t_walkable), '+',          // t_wood_floor
         TCODColor::darkOrange),
    Turf("wall",        0               , '#', TCODColor::grey),          // t_wall
    Turf("window",      mfb(t_transparent), '#', TCODColor::sky),         // t_window
    Turf("water",       mfb(t_transparent), '~', TCODColor::lightBlue),   // t_water
};


#endif // TURF_H_INCLUDED

