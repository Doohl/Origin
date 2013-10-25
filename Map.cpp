
#include "Map.h"
#include "Entity.h"
#include "Game.h"
#include <vector>
#include <iostream>


Map::Map() {
}

Map::~Map() {
    delete Field; // deallocate Field
}

void Map::Initialize(int w, int h, Turf t, Game* g) {
    // width x height (x, y)
    // Initialize the map
    grid.resize(w); // create width rows of vectors
    for(int i = 0; i < w; i++) {
        grid[i].resize(h); // create empty Entity cells
    }
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            grid[i][j] = t; // The map is filled with copies of turf T
            grid[i][j].x = i;
            grid[i][j].y = j;
        }
    }

    width = w;
    height = h;

    game = g;

    Field = new TCODMap(width, height);
}

std::vector<Entity*> Map::GetEntitiesAt(int x, int y, e_type typefilter) {

    std::vector<Entity*> ReturnEntities;

    if(x >= 0 && y >= 0) { // values cannot be negative
        if(x <= width && y <= height) { // make sure the coordinates are not out of bounds!
            for(int i = 0; i < grid[x][y].contents.size(); i++) {
                // if typefilter is not specified, or typefilter matches with the entity...
                if(typefilter == null || typefilter == grid[x][y].contents[i]->etype) {
                    ReturnEntities.push_back( grid[x][y].contents[i] );
                }
            }
        }
    }
    return ReturnEntities;
}

std::vector<Mob*> Map::GetMobsAt(int x, int y) {

    std::vector<Mob*> ReturnMobs;

    if(x >= 0 && y >= 0) { // values cannot be negative
        if(x <= width && y <= height) { // make sure the coordinates are not out of bounds!
            for(int i = 0; i < grid[x][y].mobs.size(); i++) {
                ReturnMobs.push_back( grid[x][y].mobs[i] );
            }
        }
    }
    return ReturnMobs;
}

std::vector<Turf*> Map::TurfRange(int x, int y, int range) {

    std::vector<Turf*> ReturnTurfs;

    if(x >= 0 && y >= 0 && range > 0) { // values cannot be negative
        if(x <= width && y <= height) { // make sure the coordinates are not out of bounds!

            int maxx = x + range;   // the maximum x bound
            int maxy = y + range;   // the maximum y bound
            int minx = x - range;   // the minimum x bound
            int miny = y - range;   // the minimum y bound

            for(int i = minx; i <= maxx; i++) {
                for(int j = miny; j <= maxy; j++) {
                    if( (i >= 0 && j >= 0) && (i <= width && j <= height) ) // if the location is valid
                        ReturnTurfs.push_back( &(grid[i][j]) ); // add to return vector
                }
            }
        }
    }
    return ReturnTurfs;
}

std::vector<Turf*> Map::View(int x, int y, int range) {

    std::vector<Turf*> ViewTurfs;

    // Compute the temporary FoV
    Field->computeFov(x, y, 0, true, FOV_DIAMOND);

    if(x >= 0 && y >= 0 && range > 0) { // values cannot be negative
        if(x <= width && y <= height) { // make sure the coordinates are not out of bounds!

            int maxx = x + range;   // the maximum x bound
            int maxy = y + range;   // the maximum y bound
            int minx = x - range;   // the minimum x bound
            int miny = y - range;   // the minimum y bound

            for(int i = minx; i <= maxx; i++) {
                for(int j = miny; j <= maxy; j++) {
                    if( (i >= 0 && j >= 0) && (i <= width && j <= height) ) { // if the location is valid
                        if(Field->isInFov(i, j))
                            ViewTurfs.push_back( &(grid[i][j]) ); // add to return vector
                    }
                }
            }
        }
    }
    return ViewTurfs;
}

Turf* Map::At(int x, int y) {
    return &grid[x][y];
}

void Map::InsertEntity(int x, int y, Entity* entity) {

    if(x >= 0 && y >= 0) {
        if(x <= width && y <= height) {
            grid[x][y].contents.push_back(entity); // add to entities

            // Sort the contents' display layers:
            grid[x][y].LayerContents();
        }
    }
}

void Map::InsertMob(int x, int y, Mob* m) {

    if(x >= 0 && y >= 0) {
        if(x <= width && y <= height) {
            grid[x][y].contents.push_back(m);     // add to mobs
            grid[x][y].mobs.push_back(m);     // add to mobs
            m->turf = &grid[x][y];  // assign the mob the located turf

            grid[x][y].LayerContents();
        }
    }
}

void Map::SpawnTurf(int x, int y, Turf t) {
    if(x >= 0 && y >= 0) {
        if(x <= width && y <= height) {

            if(t.name == "grass") { // random grass tiles
                t.c_symbol = special_chars[game->RandomGen->get(0,3)];
            }
            t.map = this;
            t.x = x;
            t.y = y;
            grid[x][y] = t;
            Field->setProperties( x, y, (t.flags & mfb(t_transparent)), (t.flags & mfb(t_walkable)) );

        }
    }
}

void Map::SpawnMob(int x, int y, Mob m) {
    if(x >= 0 && y >= 0) {
        if(x <= width && y <= height) {
            Turf* t = &(grid[x][y]);
            Mob* placemob = new Mob();
            m.CopyTo(placemob);
            placemob->x = x;
            placemob->y = y;

            placemob->turf = t;

            t->mobs.push_back(placemob);
            t->contents.push_back(placemob);

            t->LayerContents();
        }

    }
}

void Map::SpawnItem(int x, int y, Item i) {
    if(x >= 0 && y >= 0) {
        if(x <= width && y <= height) {
            Turf* t = &(grid[x][y]);
            Item* placeitem = new Item();
            i.CopyTo(placeitem);
            placeitem->x = x;
            placeitem->y = y;

            placeitem->container = t;

            t->items.push_back(placeitem);
            t->contents.push_back(placeitem);
        }
    }
}

std::vector<Entity*> Map::GetAllEntities() {
    std::vector<Entity*> ReturnEntities;

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {

            // Loop through the selected turf's entities
            for(int k = 0; k < grid[i][j].contents.size(); k++) {

                ReturnEntities.push_back(grid[i][j].contents[k]);
            }
        }
    }
    return ReturnEntities;
}

std::vector<Mob*> Map::GetAllMobs() {
    std::vector<Mob*> ReturnMobs;

    for(int i = 0; i < grid.size(); i++) {
        for(int j = 0; j < grid[i].size(); j++) {

            // Loop through the selected turf's entities
            for(int k = 0; k < grid[i][j].mobs.size(); k++) {

                ReturnMobs.push_back(grid[i][j].mobs[k]);
            }
        }
    }
    return ReturnMobs;
}

std::vector<Mob*> Map::FilterMobs(std::vector<Turf*> turfs) {
    std::vector<Mob*> ReturnMobs;

    int size = turfs.size();
    for(int i = 0; i < turfs.size(); i++) {
        if(turfs[i] != NULL) {
            // Check for extranneous mob sizes. For some reason, a select few turfs can start with a mob size of several hundred thousands, and cannot be reduced.
            // For now, check if mobsize is less than 10,000. Shouldn't pose a problem.
            if(turfs[i]->mobs.size() > 0 && turfs[i]->mobs.size() <= 100000) {
                for(int j = 0; j < turfs[i]->mobs.size(); j++) {
                    if(turfs[i]->mobs[j] != NULL) {
                        if(turfs[i]->mobs[j]->turf != NULL && !(turfs[i]->mobs[j]->deleting) )
                            ReturnMobs.push_back( turfs[i]->mobs[j] );
                    }

                }
            }
        }
    }
    return ReturnMobs;
}





