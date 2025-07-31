#ifndef COMBAT_H_
#define COMBAT_H_

#include <stdbool.h>

#include "skills.h"

struct attack_result {
    bool hit;
    bool crit;
};

enum damage_type {
    // physical
    TYPE_BLUDGEONING,
    TYPE_PIERCING,
    TYPE_SLASHING,
    // magical physical
    TYPE_M_BLUDGEONING,
    TYPE_M_PIERCING,
    TYPE_M_SLASHING,
    // elemental
    TYPE_FIRE,
    TYPE_COLD,
    TYPE_ACID,
    TYPE_THUNDER,
    TYPE_LIGHTNING,
    // others
    TYPE_RADIANT,
    TYPE_NECROTIC,
    TYPE_FORCE,
    TYPE_POISON,
    NUM_DAMAGE_TYPES
};

enum damage_modification {
    DAMAGE_NONE,
    DAMAGE_RESISTANCE,
    DAMAGE_IMMUNITY,
    DAMAGE_VULNERABILITIY
};

struct attack_result npc_make_attack(struct npc_sheet attacker,
                                     struct npc_sheet defender,
                                     enum stat attacking_stat);

void npc_apply_damage(struct npc_sheet *npc, size_t damage,
                      enum damage_type type);

// Normally, these would be loaded from a DLL or something
void combat_eldritch_blast(const struct npc_sheet *attacker,
                           struct npc_sheet *defender);
void combat_fireball(const struct npc_sheet *attacker,
                     struct npc_sheet *defenders, size_t num_defenders);
void combat_longsword_strike(const struct npc_sheet *attacker,
                             struct npc_sheet *defender);

#endif // COMBAT_H_
