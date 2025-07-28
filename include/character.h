#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "combat.h"
#include "skills.h"
#include <stdint.h>

struct npc_sheet {
    uint8_t stats[NUM_STATS];

    uint16_t AC, HP;

    enum damage_modification damage_modifications[NUM_DAMAGE_TYPES];

    // bitfields
    uint32_t proficiencies : NUM_SKILLS;
    uint32_t expertise : NUM_SKILLS;
    uint8_t saves : NUM_STATS;

    // actually CR / 8, determines proficiency bonus
    uint8_t CR;
};

#endif // CHARACTER_H_
