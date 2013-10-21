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

        /* Copies all this mob's relevant variables to another mob */
        void CopyTo(Mob* m);

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

        int mob_type;       // m_type enumerator, determines what mob type this mob is!
        int HP;             // Hit points
        int Max_HP;         // Maximum hitpoints
        int Ether;          // Mana points
        int Max_Ether;      // Maximum mana points

        unsigned int steps; // # of steps taken
        float tenergy;  // current energy
        float speed;    // energy gained per tick

        // Combat variables:
        std::vector<std::string> hostile;  // list of mob ids the mob is hostile to
        std::vector<std::string> friendly; // list of mob ids the mob identifies as friendly (can be players) (automatically adds own id)
        unsigned int aggro; // ticks needed for aggro to "cool down" if it's not currently fighting
                            // if > 0, is aggroed.
        int aggrofield; // the size of the field to attack hostiles
        Mob* target;        // a mob the current mob is focused on. Not always for combat!

        // Stats:
        int strength;       // mob's strength
        int intelligence;   // mob's intelligence
        int dexterity;      // mob's dexterity
        int perception;     // mob's perception
        int constitution;   // mob's constitution
        int spirit;         // mob's spirit


        Turf* turf;         // Memory address pointing to turf mob is currently located in

    private:
        void Deinitialize();     // De-initializes everything
};

#endif // MOB_H_INCLUDED
