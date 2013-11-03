#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED

#include "Entity.h"
#include <vector>

class Turf; // "Turf.h"

#define HOSTILE_FLAG std::string("all_not_self")

/*
    If a new variable is added or changed, be sure to initialize it in the constructor(s), initializers
    and copiers if appropriate.
*/

class Mob : public Entity {
    public:
        Mob();
        ~Mob();

        /* Initializes all starting / null values */
        virtual void init();

        /* Assigns all the values to this mob */
        void init_vals(std::string pname, std::string pid, char psymbol, TCODColor pcolor, int pMax_HP, int pMax_Ether, float pspeed,
                       std::string pdesc, std::string pgroups, std::string hostiles, std::string friendlies,
                       int paggrofield);

        /* The code executed when mob has enough energy to "move" */
        virtual void DoLogic(Game* game);

        /* Handles placement to another block, takes obstacles into account */
        virtual void Move(int newx, int newy);

        /* Modifies x/y values, takes obstacles into account */
        virtual void Step(int stepx, int stepy);

        /* Attacks a mob */
        virtual void Attack(Mob* m, Game* game);

        /* Extension of DoLogic; handles all AI-related things */
        void AI(Game* game);

        /* Step the mob towards another target Entity */
        void StepTo(Entity* targ, bool attack, Game* game);

        unsigned int steps; // # of steps taken
        float tenergy;  // current energy

        // Combat variables:
        std::vector<std::string> hostile;  // list of mob ids the mob is hostile to
        std::vector<std::string> friendly; // list of mob ids the mob identifies as friendly (can be players) (automatically adds own id)
        unsigned int aggro; // ticks needed for aggro to "cool down" if it's not currently fighting
                            // if > 0, is aggroed
        Mob* target;        // a mob the current mob is focused on. Not always for combat!

        Turf* turf;         // Memory address pointing to turf mob is currently located in

    private:
        void Deinitialize();     // De-initializes everything
};

#endif // MOB_H_INCLUDED
