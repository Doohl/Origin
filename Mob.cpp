#include "Mob.h"
#include "Turf.h"
#include "Map.h"
#include "Game.h"
#include "Player.h"
#include "Helper.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Mob::Mob() {
    init();
}

void Mob::init() {
    layer = 3;
    etype = mob;
    target = NULL;
    deleting = 0;
    //name = "NULL";
    //id = "NULL";
    steps = 0;
    //speed = 0.0f;
    tenergy = 0.0f;
    flags = 0;
    //aggrofield = 0;
    flags = 0;
    target = NULL;
    turf = NULL;
    //strength = 0;
    //intelligence = 0;
    //dexterity = 0;
    //perception = 0;
    //constitution = 0;
    //spirit = 0;
}

void Mob::init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pMax_HP, int pMax_Ether, float pspeed, std::string pdesc, std::string pgroups, std::string hostiles, std::string friendlies, int paggrofield) {
    //name = pname;
    //id = pid;
    symbol = psymbol;
    color = pcolor;
    //Max_HP = pMax_HP;
    //HP = Max_HP;
    //Max_Ether = pMax_Ether;
    //speed = pspeed;
    //desc = pdesc;
    hostile = Helper::Explode(';', hostiles);
    friendly = Helper::Explode(';', friendlies);
    if(!Helper::Find(friendly, get_property<std::string>("id"))) { // add self to friendlies
        friendly.push_back(get_property<std::string>("id"));
    }

    //aggrofield = paggrofield;
}

Mob::~Mob() {
    Deinitialize();
}

void Mob::Deinitialize() {
    if(turf != NULL) {
        if(!turf->deleting && turf->map != NULL && turf->map->Field != NULL) {
            turf->RemoveContents(this, false);
            turf->RemoveMobs(this, false);
            turf->map->Field->setProperties( turf->x, turf->y, !Helper::Find(turf->groups, std::string("opaque")), !Helper::Find(turf->groups, std::string("dense")));
        }
        turf = NULL;
    }
    groups.push_back("nologic"); // stop this from updating
    deleting = true;
    x = -1;
    y = -1;
}

void Mob::Move(int newx, int newy) {

    int founddense = 0;
    if(newx >= 0 && newy >= 0 && newx <= turf->map->width && newy <= turf->map->height ) {
        if( turf->map->Field->isWalkable(newx, newy) && !founddense ) { // check the turf's map's field if the new location is undense
            x = newx;
            y = newy;

            // Remove the mob from any previous instances
            turf->RemoveContents(this, false);
            turf->RemoveMobs(this, false);

            // Reset the old turf's cell's properties
            turf->map->Field->setProperties( turf->x, turf->y, !Helper::Find(turf->groups, std::string("opaque")), !Helper::Find(turf->groups, std::string("dense")));


            // Assign the mob a new turf
            turf = turf->map->At(x, y);
            turf->contents.push_back(this);
            turf->mobs.push_back(this);
            turf->LayerContents(); // sort layers

            // Set the new turf's cell's properties
            turf->map->Field->setProperties( turf->x, turf->y, (!Helper::Find(groups, std::string("opaque"))) && (!Helper::Find(turf->groups, std::string("opaque"))), false);
        }
    }
}

void Mob::Step(int stepx, int stepy) {
    int pre_x = x;
    int pre_y = y;

    Move(pre_x + stepx, pre_y + stepy); // move the mob to the new location
    steps++; // add another step
}

void Mob::Attack(Mob* m, Game* game) {
    int damage = get_property<int>("strength") + game->RandomGen->get(1, 3);
    if(m != NULL) {
        m->delta_property("hp", -damage);
    }
    if(Player* p = dynamic_cast<Player*>(m)) {
        p->Message("The " + get_property<std::string>("name") + " attacks you for " + Helper::int2str(damage) + " damage.", TCODColor(255, 0, 0), TCODColor::black);
    }
    else {
        game->player.Message("The " + get_property<std::string>("name") + " attacked " + m->get_property<std::string>("name") + " for " + Helper::int2str(damage) + " damage.", TCODColor(153, 0, 102), TCODColor::black);

        if(m->get_property<int>("hp") <= 0) {
            game->DeleteBuff.push_back(m);
            m->Deinitialize();
        }
    }

}

void Mob::DoLogic(Game* game) {
    /* If the mob doesn't have a combat target, look for one! */
    if(target == NULL) {
        if(hostile.size() > 0 && get_property<int>("aggro_range") > 0) {

            std::vector<Turf*> SeeTurfs = turf->map->View(x, y, get_property<int>("aggro_range"));
            std::vector<Mob*> SeeMobs = turf->map->FilterMobs(SeeTurfs);

            // Find the shortest distance, select the closest mob
            int leaststeps = 10000;
            Mob* closestmob = NULL;
            for(int i = 0; i < SeeMobs.size(); i++) {

                Mob* target = SeeMobs[i];
                int t_x = target->x;
                int t_y = target->y;

                /* Calculate the number of steps from this to SeeMobs[i] */

                // First, set the cell undense so that we can path to it */
                bool prevdense = turf->map->Field->isWalkable(t_x, t_y);
                bool prevvis = turf->map->Field->isTransparent(t_x, t_y);
                turf->map->Field->setProperties( t_x, t_y, !Helper::Find(turf->groups, std::string("opaque")), true);

                // Then calculate a path to it
                TCODPath* Path = new TCODPath(turf->map->Field);
                Path->compute(x, y, t_x, t_y);
                int steps = Path->size();

                // If the path is smaller than the last, it's our new target! We also need to be able to path to it
                if(steps < leaststeps && steps != 0) {

                    // Check to see if the proper aggro conditions are met
                    if( Helper::Find(hostile, target->get_property<std::string>("id")) || (Helper::Find(hostile, HOSTILE_FLAG) && !Helper::Find(friendly, target->get_property<std::string>("id"))) ) { // if the current mob is hostile towards this mob
                        leaststeps = steps;
                        closestmob = target;
                    }

                }
                // Reset the density and transparency
                turf->map->Field->setProperties( t_x, t_y, prevvis, prevdense );

                delete Path;
            }
            target = closestmob;
        }
    }

    /* Handle all the AI */
    AI(game);

}
