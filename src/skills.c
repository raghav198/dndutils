#include "skills.h"
#include "character.h"
#include "dice.h"

#include <stdbool.h>

int skills_errno = 0;

static mod_t _prof_bonus(u8 CR) {
    if (CR <= 32) {
        return 2;
    }
    return (CR - 33) / 32 + 3;
}

static enum stat _default_stat(enum skill skill) {
    switch (skill) {
    case ATHLETICS:
        return STR;
    case ACROBATICS:
    case SLEIGHT:
    case STEALTH:
        return DEX;
    case ARCANA:
    case HISTORY:
    case INVESTIGATION:
    case NATURE:
    case RELIGION:
        return INT;
    case ANIMAL:
    case INSIGHT:
    case MEDICINE:
    case PERCEPTION:
    case SURVIVAL:
        return WIS;
    case DECEPTION:
    case INTIMIDATION:
    case PERFORMANCE:
    case PERSUASION:
        return CHA;
    default:
        skills_errno = ESKILL_INVAL;
        return NUM_STATS;
    }
}

static mod_t _stat_mod(u8 score) { return (score - 10) / 2; }

mod_t npc_get_mod(struct npc_sheet npc, enum skill skill, enum stat stat) {
    skills_errno = 0;
    if (stat == NUM_STATS) {
        stat = _default_stat(skill);
    }
    if (skills_errno)
        return -1;

    u8 prof = _prof_bonus(npc.CR);
    bool has_proficiency = (npc.proficiencies >> skill) & 1;
    bool has_expertise = (npc.expertise >> skill) & 1;
    return _stat_mod(npc.stats[stat]) + (has_proficiency ? prof : 0) +
           (has_expertise ? prof : 0);
}

roll_result_t npc_make_check(struct npc_sheet npc, enum skill skill,
                             enum stat stat) {
    mod_t mod =
        (skill == NUM_SKILLS) ? _stat_mod(stat) : npc_get_mod(npc, skill, stat);
    if (skills_errno) {
        return -1;
    }

    roll_result_t roll = dice_perform_roll(1, 20, POLICY_SUM, 0);
    return roll + mod;
}

roll_result_t npc_make_save(struct npc_sheet npc, enum stat stat,
                            enum roll_type type) {
    roll_result_t roll;
    if (type == ROLL_ADV) {
        roll = dice_perform_roll(1, 20, POLICY_KEEP_HIGH, 1);
    } else if (type == ROLL_DIS) {
        roll = dice_perform_roll(1, 20, POLICY_KEEP_LOW, 1);
    } else {
        roll = dice_perform_roll(1, 20, POLICY_SUM, 0);
    }

    mod_t mod = _stat_mod(npc.stats[stat]);
    if ((npc.saves >> stat) & 1) {
        mod += _prof_bonus(npc.CR);
    }
    return roll + mod;
}

size_t npc_calculate_dc(struct npc_sheet npc, enum stat stat) {
    return _stat_mod(npc.stats[stat]) + _prof_bonus(npc.CR) + 8;
}

enum roll_type roll_type_aggregate(enum roll_type a, enum roll_type b) {
    if (a == ROLL_FORCED_STRAIGHT || b == ROLL_FORCED_STRAIGHT)
        return ROLL_FORCED_STRAIGHT;
    if (a == ROLL_STRAIGHT)
        return b;
    if (b == ROLL_STRAIGHT)
        return a;
    if (a == ROLL_ADV && b == ROLL_DIS) {
        return ROLL_FORCED_STRAIGHT;
    }
    return a;
}
