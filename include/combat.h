#ifndef COMBAT_H_
#define COMBAT_H_

#include <stdbool.h>
#include <stdint.h>

#include "skills.h"

struct attack_result {
    bool hit;
    bool crit;
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

enum damage_modification { NONE, RESISTANCE, IMMUNITY, VULNERABILITIY };

struct attack_result npc_make_attack(struct npc_sheet attacker,
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
