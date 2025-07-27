#ifndef SKILLS_H_
#define SKILLS_H_

#include "dice.h"
#include <stdint.h>

extern int skills_errno;

#define ESKILL_INVAL 1;

typedef int8_t mod_t;
typedef uint8_t stat_value_t;

typedef enum stat { STR, CON, DEX, INT, WIS, CHA, NUM_STATS } stat_t;

typedef enum skill {
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
} skill_t;

typedef enum _class {
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
} class_t;

typedef struct npc_sheet {
    uint8_t stats[NUM_STATS];

    uint16_t AC, HP;

    // bitfields
    uint32_t proficiencies : NUM_SKILLS;
    uint32_t expertise : NUM_SKILLS;
    uint8_t saves : NUM_STATS;

    // actually CR / 8, determines proficiency bonus
    uint8_t CR;

} npc_sheet_t;

mod_t npc_get_mod(npc_sheet_t npc, skill_t skill, stat_t stat);
roll_result_t npc_make_check(npc_sheet_t npc, skill_t skill, stat_t stat);
roll_result_t npc_make_save(npc_sheet_t npc, stat_t stat);
size_t npc_calculate_dc(npc_sheet_t npc, stat_t stat);

#endif // SKILLS_H_
