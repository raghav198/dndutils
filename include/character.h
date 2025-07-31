#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "combat.h"
#include "dice.h"
#include "ints.h"
#include "skills.h"

typedef u8 level_t;

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
    f32 walking;
    f32 flying;
    f32 swimming;
    f32 burrowing;
};

struct character_info {
    u16 HP;
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

    u8 spell_slots[9]; // TODO: track warlock slots separately!
    level_t hit_dice[NUM_SIZES];
};

struct npc_sheet {
    u8 stats[NUM_STATS];

    u16 AC, max_HP;

    struct movement speed;

    enum damage_modification damage_modifications[NUM_DAMAGE_TYPES];

    // bitfields
    u32 proficiencies : NUM_SKILLS;
    u32 expertise : NUM_SKILLS;
    u8 saves : NUM_STATS;

    // actually CR / 8, determines proficiency bonus
    u8 CR;

    // stuff that might change on a turn-by-turn basis
    struct character_info info;
};

struct subclass {
    const char *name;
};

struct pc_sheet {
    struct npc_sheet base;
    level_t levels[NUM_CLASSES];
    struct subclass subclasses[NUM_CLASSES];
};

u8 pc_get_cr(struct pc_sheet pc);
void character_start_turn(struct npc_sheet *npc);
void character_roll_hit_die(struct npc_sheet *npc, enum dice_size size);
void character_long_rest(struct pc_sheet *pc);

#endif // CHARACTER_H_
