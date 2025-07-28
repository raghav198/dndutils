#ifndef COMBAT_H_
#define COMBAT_H_

#include "skills.h"

#include <stdbool.h>

typedef struct attack_result {
    bool hit;
    bool crit;
} attack_result_t;

attack_result_t npc_make_attack(struct npc_sheet attacker,
                                struct npc_sheet defender,
                                enum stat attacking_stat);

void npc_apply_damage(struct npc_sheet *npc, size_t damage);

// Normally, these would be loaded from a DLL or something
void combat_eldritch_blast(const struct npc_sheet *attacker,
                           struct npc_sheet *defender);
void combat_fireball(const struct npc_sheet *attacker,
                     struct npc_sheet *defenders, size_t num_defenders);
void combat_longsword_strike(const struct npc_sheet *attacker,
                             struct npc_sheet *defender);

#endif // COMBAT_H_
