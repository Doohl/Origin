#include "Mob.h"
#include "Game.h"
#include "Helper.h"
#include "Turf.h"
#include <iostream>

void Mob::AI(Game* game) {

    /* Any special AI behaviors defined here: */
    if(id == "nope") {
    }

    /* Generic mob behavior: wander around, if target, attack them! */
    else {
        if(target != NULL) {

            if(!target->deleting) {

                /* Make sure the target is still in view */
                std::vector<Turf*> SeeTurfs = turf->map->View(x, y, aggrofield);
                std::vector<Mob*> SeeMobs = turf->map->FilterMobs(SeeTurfs);

                /* If in view, path towards it */
                if(Helper::Find(SeeMobs, target)) {
                    StepTo(target, true, game);

                /* Can't see the target. Just ignore it. */
                } else {
                    target = NULL;
                    Step(game->RandomGen->get(-1,1), game->RandomGen->get(-1,1));
                }

            // the target is being deleted:
            } else {
                target = NULL;
                Step(game->RandomGen->get(-1,1), game->RandomGen->get(-1,1));
            }

        } else {
            Step(game->RandomGen->get(-1,1), game->RandomGen->get(-1,1));
        }
    }
}

void Mob::StepTo(Entity* targ, bool attack, Game* game) {

    if(!targ->deleting && targ->x != -1 && targ->y != -1) {

        int t_x = targ->x;
        int t_y = targ->y;
        // Set the target's location to undense, for now.
        bool prevdense = turf->map->Field->isWalkable(t_x, t_y);
        bool prevvis = turf->map->Field->isTransparent(t_x, t_y);
        turf->map->Field->setProperties( t_x, t_y, (!(flags & mfb(m_opaque)) && (turf->map->grid[t_x][t_y].flags & mfb(t_transparent))), true );

        TCODPath* Path = new TCODPath(turf->map->Field);
        Path->compute(x, y, t_x, t_y);

        /* We can path to the target, so take a step */
        if((Path->size() <= aggrofield) && (Path->size() > 1)) {
            int newx, newy;
            Path->get(0, &newx, &newy);
            if(newx >= 0 && newy >= 0 && newx <= turf->map->width && newy <= turf->map->height)
                Move(newx, newy);
        }
        turf->map->Field->setProperties( t_x, t_y, prevvis, prevdense );

        /* We can't path to the target, so follow a straight line to target */
        if(Path->size() == 0) {
            TCODLine::init(x, y, t_x, t_y);
            int newx, newy;
            TCODLine::step(&newx, &newy);
            if(newx >= 0 && newy >= 0 && newx <= turf->map->width && newy <= turf->map->height)
                Move(newx, newy);
        }


        /* The target is right next to us! */
        if(Path->size() == 1) {

            if(Mob* m = dynamic_cast<Mob*>(targ)) { // attack mobs

                Attack(m, game);
            }
        }
        delete Path;
    }
}
