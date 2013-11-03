#include "Game.h"
#include "Entity.h"
#include "Mob.h"
#include "Item.h"
#include "Map.h"
#include "Helper.h"
#include <vector>
#include <iostream>

#include "depend/AMEFProtocol/AMEFDecoder.h"
#include "depend/AMEFProtocol/AMEFEncoder.h"
#include "depend/tinyxml2/tinyxml.h"

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

    GameMap.Initialize(100, 100, getTurf("t_wall"), this);
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
                GameMap.SpawnTurf(x, y, getTurf("t_wall"));
            }
            else if ( smap[y][x] == '~' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_water"));
            }
            else if ( smap[y][x] == 'g' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnMob(x, y, getMob("m_goat"));
            }
            else if ( smap[y][x] == 'M' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnMob(x, y, getMob("m_kelrah"));
            }
            else if ( smap[y][x] == 'S' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnMob(x, y, getMob("m_skeleton"));
            }
            else if ( smap[y][x] == '+' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_window"));
            }
            else if ( smap[y][x] == '@' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.InsertMob(x, y, &player);
                player.x = x;
                player.y = y;
                player.cam_x = player.x;
                player.cam_y = player.y;
            }
            else if ( smap[y][x] == '/' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnItem(x, y, getItem("i_shortsword"));
            }
            else if ( smap[y][x] == '[' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnItem(x, y, getItem("i_backpack"));
            }
            else if ( smap[y][x] == '?' ) {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
                GameMap.SpawnItem(x, y, getItem("i_flail"));
            }
            else {
                GameMap.SpawnTurf(x, y, getTurf("t_grass"));
            }
        }
    }
    Test = "Lol!";

    // Initialize TCOD
    //TCODConsole::setCustomFont("terminal12x12.png", TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::initRoot(VIEW_WIDTH, VIEW_HEIGHT, "Origin: Tales of Anarchy", false);
    TCODSystem::setFps(15);
    TCODConsole::setKeyboardRepeat(30, 5);

    DrawHud(true);

    std::cout << "Game Started" << std::endl << std::endl;
    LoadGame();

    while(!IsExiting()) {
        GameLoop();
    }
    SaveGame();
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



            // Draw all visible entities

            for(int j = 0; j < turfchosen->contents.size(); j++) {
                Entity* entity = turfchosen->contents[j];
                if(entity->symbol != ' ') {


                    if(fade != 1.0f) {
                        TCODConsole::root->setDefaultForeground(TCODColor::lerp(TCODColor::darkGrey, entity->color, fade));
                    }
                    else {
                        TCODConsole::root->setDefaultForeground(entity->color);
                    }

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

        mob->tenergy += mob->get_property<int>("speed"); // add some energy

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

void Game::SaveGame() {
    SavePlayer();
}

void Game::LoadGame() {
    LoadPlayer();
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

    unsigned int parse_flag = 0;

    std::cout << "[LOADING RESOURCES]" << std::endl;
    for(int i = 0; i < _resources.size(); i++) {

        const char* resource = _resources[i].c_str();

        // Found MOB_DATA; initiate mob parsing
        if(strcmp(resource, CategoryStrs[MOB_DATA]) == 0) {
            parse_flag = MOB_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found ITEM_DATA; initiate item parsing
        } else if(strcmp(resource, CategoryStrs[ITEM_DATA]) == 0) {
            parse_flag = ITEM_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found TURF_DATA; initiate turf parsing
        } else if(strcmp(resource, CategoryStrs[TURF_DATA]) == 0) {
            parse_flag = TURF_PARSE;
            std::cout << resource << std::endl;
            continue;
        // Found EFFECT_DATA; initiate effect parsing
        } else if(strcmp(resource, CategoryStrs[EFFECT_DATA]) == 0) {
            parse_flag = EFFECT_PARSE;
            std::cout << resource << std::endl;
            continue;
        }

        std::cout << resource << " ";

        std::vector< std::map<std::string, std::string> > data = Helper::SimpleXMLParse(resource);

        std::cout << "(" << data.size() << " entities located)" << std::endl;

        for(int j = 0; j < data.size(); j++) {
            std::map<std::string, std::string> datum = data[j];
            std::map<std::string, boost::any> parsed_data;
            char symbol;
            TCODColor color;
            std::vector<std::string> groups;
            std::vector<std::string> friendlies;
            std::vector<std::string> enemies;

            std::map<std::string, std::string>::iterator it;
            for(it = datum.begin(); it != datum.end(); ++it) {

                std::string index = it->first;
                std::string value = it->second;

                if(index == "symbol") {
                    symbol = Helper::str2char(value);
                } else if(index == "groups") {
                    groups = Helper::Explode(';', value);
                } else if(index == "friendy") {
                    friendlies = Helper::Explode(';', value);
                } else if(index == "hostile") {
                    enemies = Helper::Explode(';', value);
                } else if(index == "color") {
                    std::vector<std::string> color_rgb = Helper::Explode(',', value);
                    color = TCODColor(Helper::str2int(color_rgb[0]), Helper::str2int(color_rgb[1]), Helper::str2int(color_rgb[2]));
                } else {
                    if(Helper::strIsInt(value)) {
                        parsed_data[index] = Helper::str2int(value);
                    } else if(Helper::strIsFloat(value)) {
                        parsed_data[index] = Helper::str2float(value);
                    } else {

                        if(index == "id")
                            friendlies.push_back(value);

                        parsed_data[index] = value;
                    }
                }
            }

            if(parse_flag == MOB_PARSE) {
                Mob mob_make;
                mob_make.set_properties(parsed_data);
                mob_make.color = color;
                mob_make.symbol = symbol;
                mob_make.friendly = friendlies;
                mob_make.hostile = enemies;
                mob_make.groups = groups;
                setMob(mob_make.get_property<std::string>("id"), mob_make);
                setEntity(mob_make.get_property<std::string>("id"), mob_make);
            } else if(parse_flag == ITEM_PARSE) {
                Item item_make;
                item_make.set_properties(parsed_data);
                item_make.color = color;
                item_make.symbol = symbol;
                item_make.groups = groups;
                setItem(item_make.get_property<std::string>("id"), item_make);
                setEntity(item_make.get_property<std::string>("id"), item_make);
            } else if(parse_flag == TURF_PARSE) {
                Turf turf_make;
                turf_make.set_properties(parsed_data);
                turf_make.color = color;
                turf_make.symbol = symbol;
                turf_make.groups = groups;
                setTurf(turf_make.get_property<std::string>("id"), turf_make);
                setEntity(turf_make.get_property<std::string>("id"), turf_make);
            }
        }
    }

    std::cout << "[RESOURCES LOADED (Total resources: " << _entities.size() << ")" << std::endl << std::endl;
}

void Game::SavePlayer() {
    tinyxml2::XMLDocument doc;

    // The main player root block
    tinyxml2::XMLElement* player_root = doc.NewElement("player");
    doc.LinkEndChild(player_root);

    player_root->SetAttribute("x", player.x);
    player_root->SetAttribute("y", player.y);
    player_root->SetAttribute("max_hp", player.get_property<int>("max_hp"));
    player_root->SetAttribute("hp", player.get_property<int>("hp"));
    player_root->SetAttribute("max_ether", player.get_property<int>("max_ether"));
    player_root->SetAttribute("ether", player.get_property<int>("ether"));

    if(player.inventory.size() > 0) {
        tinyxml2::XMLElement* inventory = doc.NewElement("inventory");
        player_root->LinkEndChild(inventory);

        for(int i = 0; i < player.inventory.size(); i++) {
            Item* item = player.inventory[i];
            tinyxml2::XMLElement* inventory_element = doc.NewElement("inv");
            if(player.lefthand == item) {
                inventory_element->SetName("l_hand");
            } else if(player.righthand == item) {
                inventory_element->SetName("r_hand");
            } else if(Helper::Find(player.Worn, item)) {
                inventory_element->SetName("worn");
            } else {
                inventory_element->SetName("inv");
            }
            item->SaveEntity(inventory_element);
            inventory->LinkEndChild(inventory_element);
        }
    }
    std::string directory = std::string(SAVE_DIR) + "/" + player.get_property<std::string>("name") + ".sav";

    // Save the player savefile into disk:
    Helper::Smart_MKDir(std::string(SAVE_DIR)); // make sure the directory exists first
    doc.SaveFile(directory.c_str(), false);
}

void Game::LoadPlayer() {
    std::string directory = std::string(SAVE_DIR) + "/" + player.get_property<std::string>("name") + ".sav";
    if(Helper::fexists(directory)) {
        tinyxml2::XMLDocument doc;
        doc.LoadFile(directory.c_str());
        tinyxml2::XMLElement* player_root = doc.FirstChildElement();

        int newx = 0;
        int newy = 0;
        newx = player_root->FindAttribute("x")->IntValue();
        newy = player_root->FindAttribute("y")->IntValue();
        player.Move(newx, newy);

        player.set_property("max_hp", player_root->FindAttribute("max_hp")->IntValue());
        player.set_property("hp", player_root->FindAttribute("hp")->IntValue());
        player.set_property("max_ether", player_root->FindAttribute("max_ether")->IntValue());
        player.set_property("ether", player_root->FindAttribute("ether")->IntValue());

        tinyxml2::XMLElement* inventory = player_root->FirstChildElement("inventory");
        if(inventory && inventory != doc.LastChildElement()) {
            for(tinyxml2::XMLElement* item_elem = inventory->FirstChildElement(); item_elem != NULL; item_elem = item_elem->NextSiblingElement()) {
                Item* item = new Item();
                item->LoadEntity(item_elem);
                player.InventoryLoad(item);

                std::string type = std::string(item_elem->Name());
                if(type == "worn") {
                    player.Worn.push_back(item);
                }
                if(type == "l_hand") {
                    player.lefthand = item;
                }
                if(type == "r_hand") {
                    player.righthand = item;
                }

            }
        }
    }
}
