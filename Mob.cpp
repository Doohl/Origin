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
    layer = 3;
    etype = mob;
    name = "";
    id = "";
    steps = 0;
    speed = 0.0f;
    tenergy = 0.0f;
    flags = 0;
    aggrofield = 0;
    target = NULL;
    turf = NULL;
}

Mob::Mob(std::string n, char s, TCODColor c, int maxhp, float sp, std::string description, unsigned int f, std::string hostiles, std::string friendlies, int a, int mobtype) {
    layer = 3;
    etype = mob;
    name = n;
    std::transform(n.begin(), n.end(), n.begin(), ::tolower); // transform n to lowercase, make that the id
    id = n;
    symbol = s;
    color = c;
    Max_HP = maxhp;
    HP = maxhp;
    steps = 0;
    speed = sp;
    desc = description;
    flags = f;
    tenergy = 0.0f;
    hostile = Helper::Explode(';', hostiles);
    friendly = Helper::Explode(';', friendlies);

    // Add itself to friendlies if it's not already in it
    if(!Helper::Find(friendly, id)) {
        friendly.push_back(id);
    }

    aggrofield = a;
    target = NULL;
    strength = 0;
    mob_type = mobtype;

    turf = NULL;

}

Mob::~Mob() {
    Deinitialize();
}

void Mob::Deinitialize() {
    if(turf != NULL && turf->map != NULL) {
        turf->RemoveContents(this, false);
        turf->RemoveMobs(this, false);
        turf->map->Field->setProperties( turf->x, turf->y, (turf->flags & mfb(t_transparent)), (turf->flags & mfb(t_walkable)) );
        turf = NULL;
    }
    flags |= mfb(m_skipupdate); // stop this from updating
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
            turf->map->Field->setProperties( turf->x, turf->y, (turf->flags & mfb(t_transparent)), (turf->flags & mfb(t_walkable)) );

            // Assign the mob a new turf
            turf = &(turf->map->grid[x][y]);
            turf->contents.push_back(this);
            turf->mobs.push_back(this);
            turf->LayerContents(); // sort layers

            // Set the new turf's cell's properties
            turf->map->Field->setProperties( turf->x, turf->y, (!(flags & mfb(m_opaque)) && (turf->flags & mfb(t_transparent))), false );
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
    int damage = strength + game->RandomGen->getInt(1, 3);
    if(m != NULL) {
        m->HP -= damage;
    }
    if(Player* p = dynamic_cast<Player*>(m)) {
        p->Message("The " + name + " attacks you for " + Helper::int2str(damage) + " damage.", TCODColor(255, 0, 0), TCODColor::black);
    }
    else {
        game->player.Message("The " + name + " attacked " + m->name + " for " + Helper::int2str(damage) + " damage.", TCODColor(153, 0, 102), TCODColor::black);

        if(m->HP <= 0) {
            game->DeleteBuff.push_back(m);
            m->Deinitialize();
        }
    }

}

void Mob::DoLogic(Game* game) {

    /* If the mob doesn't have a combat target, look for one! */
    if(target == NULL) {
        if((hostile.size() > 0 || flags & mfb(m_hostile)) && aggrofield > 0) {

            std::vector<Turf*> SeeTurfs = turf->map->View(x, y, aggrofield);
            std::vector<Mob*> SeeMobs = turf->map->FilterMobs(SeeTurfs);

            // Find the shortest distance, select the closest mob
            int leaststeps = 10000;
            Mob* closestmob;
            for(int i = 0; i < SeeMobs.size(); i++) {

                Mob* target = SeeMobs[i];
                int t_x = target->x;
                int t_y = target->y;

                /* Calculate the number of steps from this to SeeMobs[i] */

                // First, set the cell undense so that we can path to it */
                bool prevdense = turf->map->Field->isWalkable(t_x, t_y);
                bool prevvis = turf->map->Field->isTransparent(t_x, t_y);
                turf->map->Field->setProperties( t_x, t_y, (!(flags & mfb(m_opaque)) && (turf->map->grid[t_x][t_y].flags & mfb(t_transparent))), true );

                // Then calculate a path to it
                TCODPath* Path = new TCODPath(turf->map->Field);
                Path->compute(x, y, t_x, t_y);
                int steps = Path->size();

                // If the path is smaller than the last, it's our new target! We also need to be able to path to it
                if(steps < leaststeps && steps != 0) {

                    if( Helper::Find(hostile, target->id) || (flags & mfb(m_hostile) && !Helper::Find(friendly, target->id)) ) { // if the current mob is hostile towards this mob
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

void Mob::CopyTo(Mob* m) {
    m->etype = etype;
    m->name = name;
    m->id = id;
    m->symbol = symbol;
    m->color = color;

    m->Max_HP = Max_HP;
    m->HP = HP;
    m->Ether = Ether;
    m->Max_Ether = Max_Ether;

    m->steps = steps;
    m->speed = speed;
    m->desc = desc;
    m->flags = flags;
    m->friendly = friendly;
    m->hostile = hostile;
    m->friendly = friendly;
    m->aggrofield = aggrofield;
    m->strength = strength;
    m->intelligence = intelligence;
    m->dexterity = dexterity;
    m->perception = perception;
    m->constitution = constitution;
    m->spirit = spirit;
    m->mob_type = mob_type;
}

