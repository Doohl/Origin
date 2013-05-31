#ifndef MOBDATA_H_INCLUDED
#define MOBDATA_H_INCLUDED

// The type of mob
enum m_type {
    m_null = 0,
    m_player,

    /*  Wildlife  */
    m_wildlife,

        /*  Peaceful wildlife  */
            m_goat, m_sheep,
            m_myur,
            m_bolomini, m_bolo,
            m_cow, m_mur,
            m_squirrel,
            m_bear,
            m_deer,
            m_soviet_pangola, m_liberty_pangola,
            m_rabbit,
            m_eeko,
            m_speeko,
            m_leet,
            m_uwah,
            m_miray,
            m_beur,
            m_korkulo,
            m_pravi,


    m_wildlife_end,

    /*  Night Monsters  */
    m_skeleton,
    m_zombie,
    m_blooddemon,
    m_shade,

    num_m_type,
};


// Mob properties for a bitfield
enum m_flag {
    m_fnull = 0,
    m_opaque,                // blocks vision
    m_skipupdate,            // do not update during loop
    m_sees,                  // can see things
    m_hears,                 // can hear noise
    m_smells,                // can smell things
    m_numb,                  // can't feel things (like damage, environment changes...)
    m_warm,                  // warm body temperature
    m_cold,                  // cold body temperature (if not this or above, room temperature body)
    m_psionic_sensors,       // excellent vision, can see past barriers
    m_hostile,               // attacks anything that isn't a friend
    num_m_flag
};


// "Category" of NPC/Monster the mob is in
enum caste {
    c_null = 0,
    c_animals,           // normal animals
    c_huntanimals,       // hunter animals
    c_nightmonsters,
    c_ants,
    c_spiders,
};

#endif // MOBDATA_H_INCLUDED
