#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include "Map.h"

// The location in which to save world data in
#define WORLD_SAVE_DIR  "save/world"

// The height of a map cell
#define SECTOR_WIDTH    200
#define SECTOR_HEIGHT   200

/* A simple container for a height level in a world */

class World {

    public:
        World();
        ~World();

        // A 3-dimensional vector container holding individual Map cells.
        typedef std::vector< std::vector< std::vector< Map* > > > grid_3d;
        // A 2-dimensional vector container holding individual Map cells.
        typedef std::vector< std::vector< Map* > > grid_2d;
        // A small container depicting a single map datum property map
        typedef std::map<std::string, boost::any> map_datum;
        // A 3-dimensional vector container holding individual map datums.
        typedef std::vector< std::vector< std::vector< map_datum > > > map_data_3d;

        /* Move the world over to x and y */
        void Translate(int x, int y);

        /* Clears the current neighbor container and populates it relative to cur_map */
        void PopulateNeighbors();

        /* Move the world over delative to cur_map by dx and dy */
        void DeltaTranslate(int dx, int dy);

        /* Save the cur_map and all neighbors */
        void SaveWorld();

        /* Load the cur_map and all its neighbors */
        void LoadWorld();

        /* Generate maps that need to be based off a global range relative to cur_map */
        void GenMaps(int range, Turf fill);

        /* Save a map with the specified path */
        void SaveMap(Map* map, std::string path);

        /* Save a map */
        void SaveMap(Map* map);

        /* Get a map datum at (x, y, z) */
        map_datum getMapDatum(int x, int y, int z);

        /* Gets the HeightLevel at this z */
        //HeightLevel* getHeightLevel(int level);

        /* Gets the current neighbors */
        grid_3d getNeighbors()        { return _neighbors; }

        Map* cur_map;                     // the current map that the player is in
        Game* game;                       // the main game object

        //HeightLevel* cur_level;           // the current height level / z layer

    private:

        grid_3d _neighbors;               // maps that neighbor the current map (directly up and down as well)
            /* Relative neighbor coordinates:

                (-1, -1)     (0, -1)     (1, -1)
                (-1, 0)     (cur_map)    (1, 0)
                (-1, 1)      (0, 1)      (1, 1)

                Up:     (0, 0, 1)
                Down:   (0, 0, -1)
            */

        map_data_3d _world_map_data;      // a 3D vector which contains a standard map<std::string, boost::any> for every map cell in the world
                                          // keep basic information on all maps in existence here for easy lookup

};

#endif // WORLD_H_INCLUDED
