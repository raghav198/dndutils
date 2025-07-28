#ifndef SKILLS_H_
#define SKILLS_H_

#include "dice.h"
#include <stdint.h>

extern int skills_errno;

#define ESKILL_INVAL 1;

typedef int8_t mod_t;
typedef uint8_t stat_value_t;

struct npc_sheet;

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

mod_t npc_get_mod(struct npc_sheet npc, enum skill skill, enum stat stat);
roll_result_t npc_make_check(struct npc_sheet npc, enum skill skill,
                             enum stat stat);
roll_result_t npc_make_save(struct npc_sheet npc, enum stat stat);
size_t npc_calculate_dc(struct npc_sheet npc, enum stat stat);

#endif // SKILLS_H_
