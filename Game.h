#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

// The "state" of the game. Self explanitory
enum GameState {
    Uninitialized,
    MainMenu,
    Menu,
    Playing,
    Exiting
};

// Main console:
#define VIEW_WIDTH              90  // number of Libtcod cells wide
#define VIEW_HEIGHT             80  //         ...       cells high
#define UI_BOTTOM_DIV           60
#define MAX_MESSAGE_HEIGHT      52

// Inventory console:
#define INV_WIDTH       60
#define INV_HEIGHT      50

// Inventory information console:
#define INV_I_WIDTH     45
#define INV_I_HEIGHT    30

// The location of the resources.txt file
#define RESOURCE_DEFINE "data/resources.txt"
// The parent directory to search for resources
#define PARENT_RESOURCE "data/raw/"

// The location which to create savefiles at
#define SAVE_DIR "save"

#include "Map.h"
#include "libtcod.hpp"
#include "Player.h"
#include <map>

enum ResourceCategories {
    NULL_DATA,
    MOB_DATA,
    ITEM_DATA,
    TURF_DATA,
    EFFECT_DATA
};

// Game class, the main object that handles all game logic and stores all entites
class Game {

    public:
        Game();
        ~Game();

    /** Global Members: **/

        /* Starts the main game loop */
        void Start();

        /* Makes time go by: 100.0f = 1 second */
        void ApplyTime(float time);

        /* Saves the game entirely */
        void SaveGame();

        /* Loads the game */
        void LoadGame();

        Map GameMap;            // The game map being used (Map.h)
        TCODRandom* RandomGen;  // The random-number generator (libtcod.hpp)
        unsigned int Turns;      // The # of turns that have gone by

        std::vector<Entity*> DeleteBuff;

        float fade;

        /* Debug Members: */
        std::string Test;

    /** Resource Members: **/

        /* Returns an entity from _entities */
        Entity getEntity(std::string index) { return _entities[index]; }
        /* Returns a mob from _mobs */
        Mob getMob(std::string index)       { return _mobs[index]; }
        /* Returns an item from _items */
        Item getItem(std::string index)     { return _items[index]; }
        /* Returns a turf from _turfs */
        Turf getTurf(std::string index)     { return _turfs[index]; }


        /* Sets the entity at _entities[string] */
        void setEntity(std::string index, Entity new_val)   { _entities[index] = new_val; }
        /* Sets the mob at _mobs[string] */
        void setMob(std::string index, Mob new_val)         { _mobs[index] = new_val; }
        /* Sets the item at _items[string] */
        void setItem(std::string index, Item new_val)       { _items[index] = new_val; }
        /* Sets the turf at _turfs[string] */
        void setTurf(std::string index, Turf new_val)       { _turfs[index] = new_val; }

        /* Locate associative indexes for the respective entity resource containers */
        bool findEntity(std::string index)                  { return (_entities.find(index) != _entities.end()); }
        bool findMob(std::string index)                     { return (_mobs.find(index) != _mobs.end()); }
        bool findItem(std::string index)                    { return (_items.find(index) != _items.end()); }
        bool findTurf(std::string index)                    { return (_turfs.find(index) != _turfs.end()); }

    /** Player Members: **/             // see: Player.cpp and Controls.cpp

        /* Processes all TCOD Key and TCOD Mouse events */
        void ProcessInput(TCOD_key_t& key, TCOD_mouse_t& mouse);

        Player player; // the player object

    /** User Interface Members: **/     // see: Game_UI.cpp

        /* Draws the hud */
        void DrawHud(bool update);

        /* Draws the inventory */
        void DrawInv();

        /* Draws inventory examination info */
        void InvInfo();


    private:

    /** Internal Logic Members: **/
        /* Determines if the game should be exited this loop */
        bool IsExiting();

        /* 15 FPS; every tick calls this and renders everything that needs to be rendered. */
        void GameLoop();

        /* Called every time the player makes an action */
        void UpdateLogic(float time);

    /** Internal Resource Members: **/
        /* Load global resources from RESOURCE_DEFINE file */
        void ConstructResources();

        /* Construct entity prototypes via Helper::SimpleXMLParse */
            // NOTE: prototypes are constructed from _resources
        void ConstructPrototypes();

        // Loaded resource locations
        std::vector<std::string> _resources;
            // contains both paths and categories, categories dictated by ! delimiter

        // Entity prototype data (associative containers: [ID] = ENTITY)
        std::map< std::string, Entity > _entities;   // master container
        std::map< std::string, Mob > _mobs;          // MOB_DATA container
        std::map< std::string, Item > _items;        // ITEM_DATA container
        std::map< std::string, Turf > _turfs;        // TURF_DATA container

    /** Miscellaneous Members: **/


        GameState _gameState; // Enumerated game state

        TCODConsole* _inventoryConsole; // inventory console
        TCODConsole* _inventoryInfo;    // inventory info console

        int debug_a_code; // debugging variable, used for in-game libtcod character map browsing

};

#endif // GAME_H_INCLUDED
