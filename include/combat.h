#ifndef COMBAT_H_
#define COMBAT_H_

#include "skills.h"

#include <stdbool.h>

typedef struct attack_result {
    bool hit;
    bool crit;
} attack_result_t;

attack_result_t npc_make_attack(npc_sheet_t attacker, npc_sheet_t defender,
                                stat_t attacking_stat);

void npc_apply_damage(npc_sheet_t *npc, size_t damage);

// Normally, these would be loaded from a DLL or something
void combat_eldritch_blast(const npc_sheet_t *attacker, npc_sheet_t *defender);
void combat_fireball(const npc_sheet_t *attacker, npc_sheet_t *defenders,
                     size_t num_defenders);
void combat_longsword_strike(const npc_sheet_t *attacker,
                             npc_sheet_t *defender);

#endif // COMBAT_H_
