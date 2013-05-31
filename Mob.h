#ifndef MOB_H_INCLUDED
#define MOB_H_INCLUDED

#include "Entity.h"
#include "MobData.h"
#include <vector>

class Turf; // "Turf.h"

/*
    IMPORTANT! Please read:

    If a new variable is added or changed, be sure to initialize it in the constructor(s) if appropriate.
    Be sure to update the == overloader and Mob::CopyTo(), or things will break.
*/

class Mob : public Entity {
    public:
        Mob();
        Mob(std::string n, char s, TCODColor c, int maxhp, float sp, std::string description, unsigned int f, std::string hostiles, std::string friendlies, int a, int mobtype);
        ~Mob();

        /* The code executed when mob has enough energy to "move" */
        virtual void DoLogic(Game* game);

        /* Copies the mob's variables onto another mob */
        void CopyTo(Mob* m);

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

extern Mob moblist[num_m_type];

void InitMobs();

#endif // MOB_H_INCLUDED
