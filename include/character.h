#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "combat.h"
#include "dice.h"
#include "skills.h"
#include <stdint.h>

typedef uint8_t level_t;

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

struct movement {
    float walking;
    float flying;
    float swimming;
};

struct character_info {
    uint16_t HP;
    struct {
        float x, y;
    } pos;

    struct {
        struct movement movement;
        bool action;
        bool bonus_action;
        bool reaction;
        bool object_interaction;
    } action_economy;

    uint8_t spell_slots[9]; // TODO: track warlock slots separately!
    level_t hit_dice[NUM_SIZES];
};

struct npc_sheet {
    uint8_t stats[NUM_STATS];

    uint16_t AC, max_HP;

    struct movement speed;

    enum damage_modification damage_modifications[NUM_DAMAGE_TYPES];

    // bitfields
    uint32_t proficiencies : NUM_SKILLS;
    uint32_t expertise : NUM_SKILLS;
    uint8_t saves : NUM_STATS;

    // actually CR / 8, determines proficiency bonus
    uint8_t CR;

    // stuff that might change on a turn-by-turn basis
    struct character_info info;
};

struct pc_sheet {
    struct npc_sheet base;
    level_t levels[NUM_CLASSES];
};

uint8_t pc_get_cr(struct pc_sheet pc);
void character_start_turn(struct npc_sheet *npc);
void character_roll_hit_die(struct npc_sheet *npc, enum dice_size size);
void character_long_rest(struct pc_sheet *pc);
// level_t character_caster_level(struct pc_sheet pc);
#endif // CHARACTER_H_
