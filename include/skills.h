#ifndef SKILLS_H_
#define SKILLS_H_

#include "dice.h"
#include "ints.h"
extern int skills_errno;

#define ESKILL_INVAL 1;

typedef i8 mod_t;
typedef u8 stat_value_t;

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

enum save_type { SAVE_STRAIGHT, SAVE_ADV, SAVE_DIS };

mod_t npc_get_mod(struct npc_sheet npc, enum skill skill, enum stat stat);
roll_result_t npc_make_check(struct npc_sheet npc, enum skill skill,
                             enum stat stat);
roll_result_t npc_make_save(struct npc_sheet npc, enum stat stat,
                            enum save_type type);
size_t npc_calculate_dc(struct npc_sheet npc, enum stat stat);
enum save_type save_type_apply(enum save_type a, enum save_type b);

#endif // SKILLS_H_
