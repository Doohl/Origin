#include "World.h"

World::World() {
}

World::~World() {
}

void World::Translate(int x, int y) {
}

void World::PopulateNeighbors() {
    if(!cur_map) return;

    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            if(x == 0 && y == 0) continue; // 0,0 is our origin
            int exact_x = cur_map->x + x;
            int exact_y = cur_map->y + y;
            std::string x_str = Helper::int2str(exact_x);
            std::string y_str = Helper::int2str(exact_y);
            std::string z_str = "level_" + Helper::int2str(cur_map->z);

            // Concactinate all strings into a single map_path
                // Format: " WORLD_SAVE_DIR/z_str/x,y.map "
            std::string map_path = std::string(WORLD_SAVE_DIR) + "/" + z_str + "/" + x_str + "," + y_str + ".map";
            if(Helper::fexists(map_path)) {

            }
        }
    }
}

void World::GenMaps(int range, Turf fill) {
    if(!cur_map) return;

    for(int x = -range; x <= range; x++) {
        for(int y = -range; y <= range; y++) {
            if(x == 0 && y == 0) continue; // 0,0 is our origin
            int exact_x = cur_map->x + x;
            int exact_y = cur_map->y + y;
            std::string x_str = Helper::int2str(exact_x);
            std::string y_str = Helper::int2str(exact_y);
            std::string z_str = "level_" + Helper::int2str(cur_map->z);

            // Concactinate all strings into a single map_path
                // Format: " WORLD_SAVE_DIR/z_str/x,y.map "
            std::string map_path = std::string(WORLD_SAVE_DIR) + "/" + z_str + "/" + x_str + "," + y_str + ".map";
            if(!Helper::fexists(map_path)) {
                Helper::Smart_MKDir(std::string(WORLD_SAVE_DIR) + "/" + z_str);         // make the directories just in case

                Map newmap;
                newmap.Initialize(SECTOR_WIDTH, SECTOR_HEIGHT, fill, game);
                newmap.Generate();

                HeightLevel* level = getHeightLevel(cur_map->z);
                if(!level) {
                    level = new HeightLevel();
                    level->height = cur_map->z;
                    _levels.push_back(level);
                }
                level->AddMap(&newmap);

                SaveMap(&newmap, map_path);
            }
        }
    }
}

void World::SaveMap(Map* map, std::string path) {
}

void World::SaveMap(Map* map) {
}

HeightLevel* World::getHeightLevel(int level) {
    for(int i = 0; i < _levels.size(); i++) {
        HeightLevel* h_level = _levels[i];
        if(h_level->height == level) return h_level;
    }
    return NULL;
}

