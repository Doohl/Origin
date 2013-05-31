#include "Mob.h"

Mob moblist[num_m_type];

/* Initializes a mob in the moblist */
void MOB(std::string name, char symbol, TCODColor color, int maxhp, float speed, std::string description,
          unsigned int flags, std::string hostiles, std::string friendlies,
          int aggrofield, int mobtype) {

    moblist[mobtype] = Mob(name, symbol, color, maxhp, speed, description, flags, hostiles, friendlies, aggrofield, mobtype);
}

void InitMobs() {

    /**
        MOB() synopsis:

        #name #symbol #color #maxhp #speed
        #description
        #flags
        #hostiles #friendlies #aggrofield
        #mobtype
    */

    MOB("NULL", ' ', TCODColor::black, 0, 0.0f,
        "NULL",
        mfb(m_fnull),
        "", "", 0,
        m_null);

    MOB("Player", '@', TCODColor::white, 100, 100.0f,
        "This is you!",
        mfb(m_skipupdate)|mfb(m_sees)|mfb(m_hears)|mfb(m_smells),
        "", "", 0,
        m_player);

    MOB("goat", 'g', TCODColor::lightGrey, 15, 50.0f,
        "Just your generic goat with long, pointy horns. You can milk this for some pretty unsavory milk.",
        mfb(m_sees)|mfb(m_hears)|mfb(m_smells),
        "", "sheep", 0,
        m_goat);

    MOB("sheep", 's', TCODColor::white, 15, 50.0f,
        "A soft, small little animal with wool growing all over. You can remove the wool for some ideal clothing or furnishing material.",
        mfb(m_sees)|mfb(m_hears)|mfb(m_smells),
        "", "goat", 0,
        m_sheep);

    MOB("myur", 'M', TCODColor::desaturatedGreen, 50, 130.0f,
        "A green, bulky, furry animal that walks on all fours. It has an unusually long tail with a blue tip. It has three orange horns, with interesting designs on its fabric.",
        mfb(m_sees)|mfb(m_hears)|mfb(m_smells),
        "", "", 0,
        m_myur);

    MOB("skeleton", 'S', TCODColor::white, 15, 80.0f,
        "A walking human skeleton. It looks very fragile and not very fast.",
        mfb(m_sees)|mfb(m_hostile),
        "", "", 50,
        m_skeleton);

}
