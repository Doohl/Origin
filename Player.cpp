#include "Player.h"
#include "Turf.h"
#include "Map.h"
#include "Game.h"
#include "Helper.h"
#include <vector>
#include <iostream>
#include <sstream>

Msg::Msg() {
}

Msg::~Msg() {
}

Player::Player() {
    layer = 6;
    etype = mob;
    steps = 0;
    speed = 100.0f;
    deleting = 0;
    name = "Player";
    id = "player";
    symbol = '@';
    color = TCODColor::white;
    Max_HP = 100;
    HP = 100;
    Ether = 0;
    Max_Ether = 0;

    strength = 1;
    intelligence = 1;
    dexterity = 1;
    perception = 1;
    constitution = 1;
    spirit = 0;

    viewinginventory = false;
    selectingwear = false;
    selectingwield= false;
    rhandselected = true;
    inventorypage = 1;

    selecteditem = NULL;
    lefthand = NULL;
    righthand = NULL;

    inventory_categories["weapons"] = std::vector<Item*>();
    inventory_categories["tools"] = std::vector<Item*>();
    inventory_categories["apparel"] = std::vector<Item*>();
    inventory_categories["food"] = std::vector<Item*>();
    inventory_categories["medicine"] = std::vector<Item*>();
    inventory_categories["munitions"] = std::vector<Item*>();
    inventory_categories["misc"] = std::vector<Item*>();
}

Player::~Player() {
    if(turf != NULL) {
        turf->RemoveContents(this, false);
        turf->RemoveMobs(this, false);
        turf->map->Field->setProperties( turf->x, turf->y, (turf->flags & mfb(t_transparent)), (turf->flags & mfb(t_walkable)) );

    }
    /* De-initialize all pointer containers */

    for(int i = 0; i < msgbuff.size(); i++) {
        delete msgbuff[i];
    }
    for(int i = 0; i < inventory.size(); i++) {
        delete inventory[i];
    }
}

void Player::DoLogic(Game* game) {
}

void Player::Attack(Mob* m, Game* game) {

    int damage = strength + game->RandomGen->get(1, 3);
    if(m != NULL) {
        m->HP -= damage;
    }
    Message("You attack the " + m->name + " for " + Helper::int2str(damage) + " damage.", TCODColor(255, 0, 0), TCODColor::black);
    if(m->HP <= 0) {
        Message("The " + m->name + " dies!", TCODColor(220, 0, 0), TCODColor::black);
        delete m;
    }
}

void Player::Message(std::string txt, TCODColor fcolor, TCODColor bcolor) {

    // Delete the first element of the buffer if it gets too long
    if(msgbuff.size() >= 200) {
        delete(msgbuff[0]);
        msgbuff.erase( msgbuff.begin() );
    }

    // Split the string into words and add them seperately into the buffer
    size_t split;
    while(txt.length() > MAX_MESSAGE_HEIGHT) {
        split = txt.find_last_of(' ', MAX_MESSAGE_HEIGHT);
        if(split > MAX_MESSAGE_HEIGHT)
            split = MAX_MESSAGE_HEIGHT;

        Msg* msg = new Msg();
        msg->message = txt.substr(0, split);
        msg->fcolor = fcolor;
        msg->bgcolor = bcolor;
        msgbuff.push_back(msg);
        txt = txt.substr(split);
    }
    Msg* msg = new Msg();
    msg->message = txt;
    msg->fcolor = fcolor;
    msg->bgcolor = bcolor;
    msgbuff.push_back(msg);
}

void Player::Move(int newx, int newy) {

    if( turf->map->Field->isWalkable(newx, newy) ) { // check the turf's map's field if the new location is undense
        x = newx;
        y = newy;

        // Remove the mob from any previous instances
        turf->RemoveContents(this, false);
        turf->RemoveMobs(this, false);

        // Reset the old turf's cell's properties
        turf->map->Field->setProperties( turf->x, turf->y, (turf->flags & mfb(t_transparent)), (turf->flags & mfb(t_walkable)) );

        // Assign the mob a new turf
        turf = &(turf->map->grid[x][y]);
        turf->contents.push_back(this);
        turf->mobs.push_back(this);
        turf->LayerContents();

        // Set the new turf's cell's properties
        turf->map->Field->setProperties( turf->x, turf->y, (turf->flags & mfb(t_transparent)), false);

        cam_x = newx;
        cam_y = newy;
    }

}

void Player::Step(int stepx, int stepy, Game* g) {
    int step_x = x;
    int step_y = y;
    step_x += stepx;
    step_y += stepy;

    // Locate a mob at the new location:
    std::vector<Mob*> MobsAt = turf->map->GetMobsAt(step_x, step_y);
    if(MobsAt.size() > 0) {
        // If a mob is located, attack the last one!
        Attack(MobsAt[ MobsAt.size() - 1], g);

    // No mob located, just move.
    } else {
        Move(step_x, step_y); // move the mob to the new location
        steps++; // add another step
        //Message("You take a step.", TCODColor::white, TCODColor::black);
    }
}

char Player::InvNextChar() {
    std::vector<char> AvailableSlots = Helper::GetCharacters(); // generate available slots (a to z, A to Z and 0 to 9)
    std::vector<char> UnavailableSlots;
    char Returnchar = '~';

    for(int i = 0; i < inventory.size(); i++) {
        UnavailableSlots.push_back( inventory[i]->index );
        std::cout << inventory[i]->index << std::endl;
    }

    for(int i = 0; i < AvailableSlots.size(); i++) {
        if(!Helper::Find(UnavailableSlots, AvailableSlots[i])) {
            Returnchar = AvailableSlots[i];
            break;
        }
    }
    return Returnchar;
}

void Player::InventoryAdd(Item* item) {
    inventory.push_back(item);
    item->index = InvNextChar();
    if(Helper::Find(item->groups, std::string("weapon"))) {
        inventory_categories["weapons"].push_back(item);
    } else if(Helper::Find(item->groups, std::string("wearable"))) {
        inventory_categories["apparel"].push_back(item);
    } else if(Helper::Find(item->groups, std::string("tool"))) {
        inventory_categories["tools"].push_back(item);
    } else {
        inventory_categories["misc"].push_back(item);
    }
}

/*
std::vector<Item*> Player::SortInventory() {
    std::vector<Item*> returnvect;

    returnvect.push_back(lefthand); returnvect.push_back(righthand);

    std::map<std::string, std::vector<Item*>::const_iterator it;
    for( it = inventory_categories.begin(); it != inventory_categories.end(); ++it ) {
        std::string category = (*it).first;

        for(int i = 0; i < inventory.size(); i++) {

}
*/

void Player::InventoryRemove(Item* item, bool dodel) {
    std::vector<Item*> *category;
    if(Helper::Find(item->groups, std::string("weapon"))) {
        category = &inventory_categories["weapons"];
    } else if(Helper::Find(item->groups, std::string("wearable"))) {
        category = &inventory_categories["apparel"];
    } else if(Helper::Find(item->groups, std::string("tool"))) {
        category = &inventory_categories["tools"];
    } else {
        category = &inventory_categories["misc"];
    }
    for(int i = 0; i < category->size(); i++) {
        Item* index = (*category)[i];
        if(index == item)
            category->erase(category->begin() + i);
    }
    item->index = '~'; // revert the index to one that is never used

    for(int i = 0; i < inventory.size(); i++) {
        if(inventory[i] == item) {
            if(dodel) {
                delete inventory[i]; // delete entity from memory
            }
            inventory.erase(inventory.begin() + i); // erase the position
            break;
        }
    }

}

void Player::WornRemove(Item* item, bool dodel) {
    for(int i = 0; i < Worn.size(); i++) {
        if(Worn[i] == item) {
            if(dodel) {
                delete Worn[i]; // delete entity from memory
            }
            Worn.erase(Worn.begin() + i); // erase the position
            break;
        }
    }
}

void Player::Wear(Item* item) {
    if(!(item->isWearable())) {
        Message("You can't wear that!", TCODColor::white, TCODColor::black);
        return;
    }
    if(Helper::Find(Worn, item)) {
        Message("You're already wearing that!", TCODColor::white, TCODColor::black);
        return;
    }
    Worn.push_back(item);
    if(lefthand == item) {
        lefthand = NULL;
    }
    if(righthand == item) {
        righthand = NULL;
    }

    std::string wearmsg = "You wear ";
    if(!Helper::proper(item->name)) { // if not a proper noun, apply correct grammar
        wearmsg += "the ";
    }
    Message(wearmsg + item->name + ".", TCODColor::white, TCODColor::black);
}

void Player::Wield(Item* item) {

    if(lefthand == item || righthand == item) {
        Message("You cannot wield the wielded.", TCODColor::white, TCODColor::black);
        return;
    }
    if(rhandselected) {
        if(righthand != NULL) {
            Message("Your right hand is full.", TCODColor::white, TCODColor::black);
            return;
        }
        righthand = item;
    } else {
        if(lefthand != NULL) {
            Message("Your left hand is full.", TCODColor::white, TCODColor::black);
            return;
        }
        lefthand = item;
    }
    WornRemove(item, false); // remove it from the worn list if it's there

    std::string wearmsg = "You wield ";
    if(!Helper::proper(item->name)) { // if not a proper noun, apply correct grammar
        wearmsg += "the ";
    }
    Message(wearmsg + item->name + ".", TCODColor::white, TCODColor::black);
}

int Player::CalcMaxVolume() {
    int returnvolume = 0;
    for(int i = 0; i < Worn.size(); i++) {
        Item* item = Worn[i];
        returnvolume += item->max_volume;
    }
    return returnvolume;
}

int Player::CalcCarryVolume() {
    int returnvolume = 0;
    for(int i = 0; i < inventory.size(); i++) {
        Item* item = inventory[i];
        if(!Helper::Find(Worn, item) && righthand != item && lefthand != item)
            returnvolume += item->max_volume;
    }
    return returnvolume;
}




