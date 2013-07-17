#include "Game.h"
#include "Entity.h"
#include "Mob.h"
#include "Item.h"
#include "Map.h"
#include "Helper.h"
#include <vector>
#include <iostream>

Game::Game() {
    _gameState = Uninitialized;
    debug_a_code = 0;
    fade = 1.0f;
}

Game::~Game() {
    delete RandomGen; // release memory

    for(unsigned int i = 0; i < DeleteBuff.size(); i++) {
        delete(DeleteBuff[i]);
        DeleteBuff.erase( DeleteBuff.begin() + i );
    }
}

void Game::Start() {

    if(_gameState != Uninitialized) {
        return;
    }

    /* Initialize all prototype arrays and game objects */
    InitItems(); // initialize the itemlist array
    InitMobs();  // initialize the moblist array

    _gameState = Playing;
    _inventoryConsole = NULL;
    _inventoryInfo = NULL;
    Turns = 0;

    // Initialize player:
    player.cam_follow = true;
    player.speed = 100.0f;
    player.viewinginventory = false;

    player.Message("Have a secure day!", TCODColor::green, TCODColor::black);

    GameMap.Initialize(100, 100, turflist[t_wall], this);
    RandomGen = new TCODRandom();


    const char* smap[] = {
		"##############################################",
		"#######################      #################",
		"#####################    #     ###############",
		"######################  ###        ###########",
		"##################      #####             ####",
		"################       ########    ###### ####",
		"###############      #################### ####",
		"################    ######                  ##",
		"########   #######  ######   #     #     #  ##",
		"########   ######      ###                  ##",
		"########                                    ##",
		"####       ######      ###   #     #     #  ##",
		"#### ###   ########## ####                  ##",
		"#### ###   ##########   ########### ##########",
		"#### ##################   #####          #####",
		"#### ###             #### #####          #####",
		"####           #     ####                #####",
		"########       #     #### #####          #####",
		"########       #####      #######      #######",
		"#########++ ++####################++++########",
		"##                                          ##",
		"#                                            #",
		"#                                      g     #",
		"#                                     g g    #",
		"#       M                                    #",
		"#                        @                   #",
		"#                                      g     #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                S                           #",
		"#              S S S                         #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#   //////////////////// /  //////////////// #",
		"#                    /   /      [            #",
		"#                       / ?                  #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#                                            #",
		"#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#",
		"##~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~##",
		"##############################################",


	};
    for (int y=0; y < 54; y++ ) {
        for (int x=0; x < 46; x++ ) {
            if ( smap[y][x] == '#' ) {
                GameMap.SpawnTurf(x, y, turflist[t_wall]);
            }
            else if ( smap[y][x] == '~' ) {
                GameMap.SpawnTurf(x, y, turflist[t_water]);
            }
            else if ( smap[y][x] == 'g' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnMob(x, y, moblist[m_goat]);
            }
            else if ( smap[y][x] == 'M' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnMob(x, y, moblist[m_myur]);
            }
            else if ( smap[y][x] == 'S' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnMob(x, y, moblist[m_skeleton]);
            }
            else if ( smap[y][x] == '+' ) {
                GameMap.SpawnTurf(x, y, turflist[t_window]);
            }
            else if ( smap[y][x] == '@' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.InsertMob(x, y, &player);
                player.x = x;
                player.y = y;
                player.cam_x = player.x;
                player.cam_y = player.y;
            }
            else if ( smap[y][x] == '/' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnItem(x, y, itemlist[i_shortsword]);
            }
            else if ( smap[y][x] == '[' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnItem(x, y, itemlist[i_backpack]);
            }
            else if ( smap[y][x] == '?' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnItem(x, y, itemlist[i_dildo]);
            }
            else {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
            }
        }
    }
    Test = "Lol!";

    // Initialize TCOD
    TCODConsole::initRoot(VIEW_WIDTH, VIEW_HEIGHT, "Origin: Tales of Anarchy", false);
    TCODSystem::setFps(15);
    TCODConsole::setKeyboardRepeat(30, 5);

    DrawHud(true);

    std::cout << "Game Started" << std::endl << std::endl;
    while(!IsExiting()) {
        GameLoop();
    }
}

bool Game::IsExiting() {
    if(_gameState == Exiting || TCODConsole::isWindowClosed()) {
        return true;
    } else {
        return false;
    }
}

void Game::ApplyTime(float time) {
    // Update the game
    UpdateLogic(time);

    DrawHud(true); // Update the hud
}

void Game::GameLoop() {
    // Game is going: handle all rendering! Listens for player input to update game.

    // Compute the FoV
    GameMap.Field->computeFov(player.x, player.y, 0, true, FOV_DIAMOND); // basic FoV

    // Loop through all turfs and draw them
    std::vector<Turf*> Turfs = GameMap.TurfRange(player.cam_x, player.cam_y, 44);

    // Apply gradual fade as appropriate
    if(player.viewinginventory) {
        fade -= 0.02f;
        if(fade < 0.0f) {
            fade = 0.0f;
        }
    }
    else {
        fade = 1.0f;
    }

    for(int i = 0; i < Turfs.size(); i++) {

        Turf* turfchosen = Turfs[i]; // select the turf
        // Calculate the scrolling offset
        int x = (turfchosen->x - player.cam_x) + VIEW_WIDTH / 2;
        int y = (turfchosen->y - player.cam_y) + (VIEW_HEIGHT-16) / 2;

        if(y > 54)       // do not draw anything on the bottom 16 cells
            continue;

        // Draw the turf

        if( GameMap.Field->isInFov(turfchosen->x, turfchosen->y) && (x >= 0 && y >= 0) && (x < VIEW_WIDTH && y < VIEW_HEIGHT) ) {

            if(fade != 1.0f) {
                TCODConsole::root->setDefaultForeground(TCODColor::lerp(TCODColor::darkGrey, turfchosen->color, fade));
            }
            else {
                TCODConsole::root->setDefaultForeground(turfchosen->color);
            }

            if(turfchosen->c_symbol > 0) {
                TCODConsole::root->putChar(x, y, turfchosen->c_symbol);
            } else {
                TCODConsole::root->putChar(x, y, turfchosen->symbol);
            }

        }

        // Draw all visible entities

        for(int j = 0; j < turfchosen->contents.size(); j++) {
            Entity* entity = turfchosen->contents[j];
            if(entity->symbol != ' ' && (GameMap.Field->isInFov(entity->x, entity->y) )) {


                if(fade != 1.0f) {
                    TCODConsole::root->setDefaultForeground(TCODColor::lerp(TCODColor::darkGrey, entity->color, fade));
                }
                else {
                    TCODConsole::root->setDefaultForeground(entity->color);
                }

                if((x >= 0 && y >= 0) && (x < VIEW_WIDTH && y < VIEW_HEIGHT)) {
                    if(entity->c_symbol > 0) {
                        TCODConsole::root->putChar(x, y, entity->c_symbol);
                    } else {
                        TCODConsole::root->putChar(x, y, entity->symbol);
                    }
                }
            }
        }
    }

    // Draw the hud!
    DrawHud(false); // draw the borders and whatnot
    DrawHud(true);  // draw the messages, stats, etc

    // Draw the inventory if necessary
    DrawInv();

    if(debug_a_code > 0) {
        TCODConsole::root->putChar(5, 5, debug_a_code); // just for easy character mapping
    }
    TCODConsole::flush(); // apply changes

    // Handle user input:
    TCOD_key_t key = {TCODK_NONE,0};
    key = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
    HandleKeys(key);

    // Release unnecessary gui pointers
    if(_inventoryConsole != NULL && !player.viewinginventory) {
        delete _inventoryConsole;
        _inventoryConsole = NULL;
    }
    if(_inventoryInfo != NULL && (!player.viewinginventory || player.selecteditem == NULL)) {
        delete _inventoryInfo;
        _inventoryInfo = NULL;
    }

    // Clear the screen:
    TCODConsole::root->clear();
}



void Game::UpdateLogic(float time) {
    // Updates all the game logic: one game turn has passed!

    // Clear the screen
    TCODConsole::root->setBackgroundFlag(TCOD_BKGND_SET);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    TCODConsole::root->setDefaultForeground(TCODColor::black);
    TCODConsole::root->clear();

    // Get all the mobs to update
    std::vector<Mob*> AllMobs = GameMap.GetAllMobs();

    for(int i = 0; i < AllMobs.size(); i++) {
        Mob* mob = AllMobs[i];

        /* If entity is a mob and set to skip update; then skip this mob! */
        if( mob->flags & mfb(m_skipupdate))
            continue;

        mob->tenergy += mob->speed; // add some energy

        while(mob->tenergy >= time) {
            mob->tenergy -= 100.0f; // 100.0f would be one second I guess
            mob->DoLogic(this);
        }
    }

    /* Clean out the delete buffer */
    if(DeleteBuff.size() >= 200) {
        delete(DeleteBuff[0]);
        DeleteBuff.erase( DeleteBuff.begin() );
    }
}

