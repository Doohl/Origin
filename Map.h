#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "libtcod.hpp"
#include "Entity.h"
#include "Turf.h"
#include <vector>

class Game;

class Map {

    public:
        Map();
        ~Map();

        /* Initialize the main grid vector */
        void Initialize(int w, int h, Turf t, Game* g);

        /* Get all the entities at a coordinate; optional e_type filter (use null if no filter) */
        std::vector<Entity*> GetEntitiesAt(int x, int y, e_type typefilter);


        /* Get all the mobs at a coordinate; */
        std::vector<Mob*> GetMobsAt(int x, int y);

        /* Get turfs in a square area (allocates turfs via random-access) */
        std::vector<Turf*> TurfRange(int x, int y, int range);

        /* Get all turfs in view of a coordinate */
        std::vector<Turf*> View(int x, int y, int range);

        /* Get the turf at coordinate */
        Turf* At(int x, int y);

        /* Insert an entity into a grid at x, y */ // (NOT COPY)
        void InsertEntity(int x, int y, Entity* entity);

        /* Insert a mob into a grid at x, y */
        void InsertMob(int x, int y, Mob* m);


        /* Create a copy of Turf t at x, y */ // (COPY, NOT MEMORY ADDRESS)
        void SpawnTurf(int x, int y, Turf t);

        /* Create a copy of Mob m at x, y */
        void SpawnMob(int x, int y, Mob m);

        /* Create a copy of Item i at x, y */
        void SpawnItem(int x, int y, Item i);

        /* Iterates through all grid turfs and returns their entities */
        std::vector<Entity*> GetAllEntities();

        /* Iterates through all grid turfs and returns any mobs found */
        std::vector<Mob*> GetAllMobs();

        /* Get the mobs in a list of turfs */
        std::vector<Mob*> FilterMobs(std::vector<Turf*> turfs);


        TCODMap* Field; // for libtcod calculations; used for FoV and A*

        Game* game; // the memory address of the game this map belongs to

        int width;  // width in cells
        int height; // height in cells

        int x;      // the map's x location
        int y;      // the map's y location

    private:

        std::vector<std::vector<Turf> > grid;   // 2D Vector grid representing the grid of turfs
};


#endif // MAP_H_INCLUDED
