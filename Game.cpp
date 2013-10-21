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

    /* Load resources and xml files */
    ConstructResources();   // load all files into memory
    ConstructPrototypes();  // construct entity prototypes (from xml raw files)

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
                GameMap.SpawnMob(x, y, getMob("m_goat"));
            }
            else if ( smap[y][x] == 'M' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnMob(x, y, getMob("m_kelrah"));
            }
            else if ( smap[y][x] == 'S' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnMob(x, y, getMob("m_skeleton"));
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
                GameMap.SpawnItem(x, y, getItem("i_shortsword"));
            }
            else if ( smap[y][x] == '[' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnItem(x, y, getItem("i_backpack"));
            }
            else if ( smap[y][x] == '?' ) {
                GameMap.SpawnTurf(x, y, turflist[t_grass]);
                GameMap.SpawnItem(x, y, getItem("i_flail"));
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
        int y = (turfchosen->y - player.cam_y) + (VIEW_HEIGHT-(UI_BOTTOM_DIV-(VIEW_HEIGHT/2))) / 2;

        if(y > UI_BOTTOM_DIV)       // do not draw anything on the bottom 16 cells
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
    TCOD_mouse_t mouse;
    TCODSystem::checkForEvent((TCOD_event_t)(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE),&key,&mouse);
    ProcessInput(key, mouse);

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
        if(Helper::Find(mob->groups, std::string("nologic")))
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

void Game::ConstructResources() {
    std::vector<std::string> lines = Helper::SimpleParse(RESOURCE_DEFINE);
    for(int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];

        // Found a comment
        if(line.substr(0, 1) == "#") continue;

        // Cull out empty spaces and tabs
        Helper::replace_all(line, std::string(" "), std::string(""));  // space
        Helper::replace_all(line, std::string("\t"), std::string("")); // tab
        Helper::replace_all(line, std::string("+"), std::string(""));  // we don't need + signs either

        // We are expecting valid data
        if(line.size() > 0) {
            if(line.substr(0, 1) != "!") {
                line = PARENT_RESOURCE + line; // prefix the parent resource directory to the resource entry
            }

            _resources.push_back(line);

        }

    }
}

void Game::ConstructPrototypes() {

    const char* CategoryStrs[] = {
        "!NULL_DATA",
        "!MOB_DATA",
        "!ITEM_DATA",
        "!TURF_DATA",
        "!EFFECT_DATA"
    };

    unsigned int parse_flags = 0;

    std::cout << "[LOADING RESOURCES]" << std::endl;
    for(int i = 0; i < _resources.size(); i++) {

        const char* resource = _resources[i].c_str();

        // Found MOB_DATA; initiate mob parsing
        if(strcmp(resource, CategoryStrs[MOB_DATA]) == 0) {
            parse_flags = 0;
            parse_flags |= MOB_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found ITEM_DATA; initiate item parsing
        } else if(strcmp(resource, CategoryStrs[ITEM_DATA]) == 0) {
            parse_flags = 0;
            parse_flags |= ITEM_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found TURF_DATA; initiate turf parsing
        } else if(strcmp(resource, CategoryStrs[TURF_DATA]) == 0) {
            parse_flags = 0;
            parse_flags |= TURF_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found EFFECT_DATA; initiate effect parsing
        } else if(strcmp(resource, CategoryStrs[EFFECT_DATA]) == 0) {
            parse_flags = 0;
            parse_flags |= EFFECT_PARSE;
            std::cout << resource << std::endl;
            continue;
        }

        std::cout << resource << " ";

        std::vector< std::map<std::string, std::string> > data = Helper::SimpleXMLParse(resource, parse_flags);

        std::cout << "(" << data.size() << " entities located)" << std::endl;

        for(int j = 0; j < data.size(); j++) {
            std::map<std::string, std::string> datum = data[j];

            /* Generic entity values */
            std::string name = datum.find("name") != datum.end() ? datum["name"] : "NULL";
            std::string id = datum.find("id") != datum.end() ? datum["id"] : "NULL";
            std::string desc = datum.find("desc") != datum.end() ? datum["desc"] : "NULL";
            std::string groups = datum.find("groups") != datum.end() ? datum["groups"] : "";

            char symbol = datum.find("symbol") != datum.end() ? Helper::str2char(datum["symbol"]) : ' ';

            // Color parsing
            std::string color_str = datum.find("color") != datum.end() ? datum["color"] : "0,0,0";
                std::vector<std::string> color_rgb = Helper::Explode(',', color_str);
                TCODColor color = TCODColor(Helper::str2int(color_rgb[0]), Helper::str2int(color_rgb[1]), Helper::str2int(color_rgb[2]));

            /* Parsing mobs */
            if(parse_flags & MOB_PARSE) {
                std::string hostile = datum.find("hostile") != datum.end() ? datum["hostile"] : "";
                std::string friendly = datum.find("friendlies") != datum.end() ? datum["friendlies"] : "";

                int Max_HP = datum.find("hp") != datum.end() ? Helper::str2int(datum["hp"]) : 0;
                int Max_Ether = datum.find("ether") != datum.end() ? Helper::str2int(datum["ether"]) : 0;
                int aggrofield = datum.find("seerange") != datum.end() ? Helper::str2int(datum["seerange"]) : 0;
                int blunt = datum.find("blunt") != datum.end() ? Helper::str2int(datum["blunt"]) : 0;
                int cut = datum.find("cut") != datum.end() ? Helper::str2int(datum["cut"]) : 0;
                int pierce = datum.find("pierce") != datum.end() ? Helper::str2int(datum["pierce"]) : 0;

                float speed = datum.find("speed") != datum.end() ? Helper::str2float(datum["speed"]) : 0;

                // Assemble the Mob prototype and push it to the global associative container
                Mob mob_make;
                mob_make.init_vals(name, id, symbol, color, Max_HP, Max_Ether, speed, desc, groups, hostile, friendly, aggrofield);
                setMob(mob_make.id, mob_make);
                setEntity(mob_make.id, mob_make);
            }

            /* Parsing items */
            if(parse_flags & ITEM_PARSE) {
                int weight = datum.find("weight") != datum.end() ? Helper::str2int(datum["weight"]) : 0;
                int volume = datum.find("volume") != datum.end() ? Helper::str2int(datum["volume"]) : 0;
                int rarity = datum.find("rarity") != datum.end() ? Helper::str2int(datum["rarity"]) : 0;
                int value = datum.find("value") != datum.end() ? Helper::str2int(datum["value"]) : 0;
                int pliancy = datum.find("pliancy") != datum.end() ? Helper::str2int(datum["pliancy"]) : 0;
                int speed = datum.find("speed") != datum.end() ? Helper::str2int(datum["speed"]) : 0;
                int modifier = datum.find("modifier") != datum.end() ? Helper::str2int(datum["modifier"]) : 0;
                int quantity = datum.find("quantity") != datum.end() ? Helper::str2int(datum["quantity"]) : 0;
                int capacity = datum.find("capacity") != datum.end() ? Helper::str2int(datum["capacity"]) : 0;

                int blunt = datum.find("blunt") != datum.end() ? Helper::str2int(datum["blunt"]) : 0;
                int cut = datum.find("cut") != datum.end() ? Helper::str2int(datum["cut"]) : 0;
                int pierce = datum.find("pierce") != datum.end() ? Helper::str2int(datum["pierce"]) : 0;

                Item item_make;
                item_make.init_vals(name, id, symbol, color, modifier, quantity, weight, volume, value, rarity, blunt, cut, pierce, pliancy, 1, capacity, desc, groups);
                setItem(item_make.id, item_make);
                setEntity(item_make.id, item_make);
            }
        }
    }

    std::cout << "[RESOURCES LOADED (Total resources: " << _entities.size() << ")" << std::endl << std::endl;
}

