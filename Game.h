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
#define VIEW_WIDTH      90  // number of Libtcod cells wide
#define VIEW_HEIGHT     70  //         ...       cells high

// Inventory console:
#define INV_WIDTH       60
#define INV_HEIGHT      50

// Inventory information console:
#define INV_I_WIDTH     45
#define INV_I_HEIGHT    30

#include "Map.h"
#include "libtcod.hpp"
#include "Player.h"

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

        Map GameMap;            // The game map being used (Map.h)
        TCODRandom* RandomGen;  // The random-number generator (libtcod.hpp)
        unsigned int Turns;      // The # of turns that have gone by

        std::vector<Entity*> DeleteBuff;

        float fade;

        /* Debug Members: */
        std::string Test;

    /** Player Members: **/             // see: Player.cpp and Controls.cpp

        /* Responds to keypresses */
        void HandleKeys(TCOD_key_t& key);

        Player player; // the player object

    /** User Interface Members: **/     // see: Game_UI.cpp

        /* Draws the hud */
        void DrawHud(bool update);

        /* Draws the inventory */
        void DrawInv();

        /* Draws inventory examination info */
        void InvInfo();


    private:

        /* Determines if the game should be exited this loop */
        bool IsExiting();

        /* 15 FPS; every tick calls this and renders everything that needs to be rendered. */
        void GameLoop();

        /* Called every time the player makes an action */
        void UpdateLogic(float time);

        GameState _gameState; // Enumerated game state

        TCODConsole* _inventoryConsole; // inventory console
        TCODConsole* _inventoryInfo;    // inventory info console

        int debug_a_code; // debugging variable, used for in-game libtcod character map browsing

};

#endif // GAME_H_INCLUDED
