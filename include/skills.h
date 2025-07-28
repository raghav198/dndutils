#ifndef SKILLS_H_
#define SKILLS_H_

#include "dice.h"
#include <stdint.h>

extern int skills_errno;

#define ESKILL_INVAL 1;

typedef int8_t mod_t;
typedef uint8_t stat_value_t;

enum stat { STR, CON, DEX, INT, WIS, CHA, NUM_STATS };

enum skill {
    ATHLETICS,
    ACROBATICS,
    ANIMAL,
    ARCANA,
    DECEPTION,
    HISTORY,
    INSIGHT,
    INTIMIDATION,
    INVESTIGATION,
    MEDICINE,
    NATURE,
    PERCEPTION,
    PERFORMANCE,
    PERSUASION,
    RELIGION,
    SLEIGHT,
    STEALTH,
    SURVIVAL,
    NUM_SKILLS
};

enum class_name {
    ARTIFICER,
    BARBARIAN,
    BARD,
    CLERIC,
    DRUID,
    FIGHTER,
    MONK,
    PALADIN,
    RANGER,
    ROGUE,
    SORCERER,
    WARLOCK,
    WIZARD,
    NUM_CLASSES
};

enum damage_type {
    // physical
    BLUDGEONING,
    PIERCING,
    SLASHING,
    // magical physical
    M_BLUDGEONING,
    M_PIERCING,
    M_SLASHING,
    // elemental
    FIRE,
    COLD,
    ACID,
    THUNDER,
    LIGHTNING,
    // others
    RADIANT,
    NECROTIC,
    FORCE,
    POISON,
    NUM_DAMAGE_TYPES
};

struct npc_sheet {
    uint8_t stats[NUM_STATS];

    uint16_t AC, HP;

    // bitfields
    uint32_t proficiencies : NUM_SKILLS;
    uint32_t expertise : NUM_SKILLS;
    uint8_t saves : NUM_STATS;

    // actually CR / 8, determines proficiency bonus
    uint8_t CR;
};

mod_t npc_get_mod(struct npc_sheet npc, enum skill skill, enum stat stat);
roll_result_t npc_make_check(struct npc_sheet npc, enum skill skill,
                             enum stat stat);
roll_result_t npc_make_save(struct npc_sheet npc, enum stat stat);
size_t npc_calculate_dc(struct npc_sheet npc, enum stat stat);

#endif // SKILLS_H_
