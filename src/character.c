#include "character.h"
#include "dice.h"
#include "util.h"
#include <string.h>

static enum dice_size _hit_die_size(enum class_name cls) {
    switch (cls) {
    case SORCERER:
    case WIZARD:
        return D6;
    case ARTIFICER:
    case BARD:
    case CLERIC:
    case DRUID:
    case MONK:
    case ROGUE:
    case WARLOCK:
        return D8;
    case FIGHTER:
    case PALADIN:
    case RANGER:
        return D10;
    case BARBARIAN:
        return D12;
    default:
        return -1;
    }
}

u8 pc_get_cr(struct pc_sheet pc) {
    size_t cr = 0;
    for (size_t i = 0; i < NUM_CLASSES; i++) {
        cr += 8 * pc.levels[i];
    }
    return cr;
}

void character_start_turn(struct npc_sheet *npc) {
    npc->info.action_economy.movement = npc->speed;
    npc->info.action_economy.action = true;
    npc->info.action_economy.bonus_action = true;
    npc->info.action_economy.reaction = true;
}

void character_roll_hit_die(struct npc_sheet *npc, enum dice_size size) {
    if (npc->info.hit_dice[size] == 0)
        return;

    roll_result_t health_recovered =
        dice_perform_roll(1, dice_get_sides(size), POLICY_SUM, 0);
    health_recovered += npc_get_mod(*npc, NUM_SKILLS, CON);
    DND_INCREASE_CLAMPED(&npc->info.HP, health_recovered, npc->max_HP);
    npc->info.hit_dice[size]--;
}

static level_t _caster_level(struct pc_sheet pc) {
    level_t full_levels = pc.levels[BARD] + pc.levels[DRUID] +
                          pc.levels[CLERIC] + pc.levels[SORCERER] +
                          pc.levels[WIZARD];
    level_t half_levels =
        pc.levels[ARTIFICER] + pc.levels[PALADIN] + pc.levels[RANGER];
    level_t third_levels = 0;
    if (!strcmp(pc.subclasses[FIGHTER].name, "eldritch_knight"))
        third_levels++;
    if (!strcmp(pc.subclasses[ROGUE].name, "arcane_trickster"))
        third_levels++;
    return full_levels + (half_levels / 2) + (third_levels / 3);
}

static u8 _num_slots(level_t caster_level, u8 slot_level) {
    if (slot_level > (caster_level + 1) / 2)
        return 0;
    switch (slot_level) {
    case 1:
        return (caster_level > 3) ? 4 : caster_level + 1;
    case 2:
        return (caster_level == 3) ? 2 : 3;
    case 3:
        return (caster_level == 5) ? 2 : 3;
    case 4:
        return (caster_level > 10) ? 3 : caster_level - 6;
    case 5:
        return (caster_level == 9) ? 1 : (caster_level > 17) ? 3 : 2;
    case 6:
        return (caster_level > 18) ? 2 : 1;
    case 7:
        return (caster_level == 20) ? 2 : 1;
    case 8:
    case 9:
        return 1;
    default:
        return 0;
    }
}

void character_long_rest(struct pc_sheet *pc) {
    // Recover all HP
    pc->base.info.HP = pc->base.max_HP;

    // Calculate max # of hit dice based on class levels
    level_t max_hit_dice[NUM_SIZES] = {0};
    for (enum class_name cls = 0; cls < NUM_CLASSES; cls++) {
        max_hit_dice[_hit_die_size(cls)] += pc->levels[cls];
    }

    // Recover half your hit dice
    for (enum dice_size sz = 0; sz < NUM_SIZES; sz++) {
        DND_INCREASE_CLAMPED(pc->base.info.hit_dice + sz, max_hit_dice[sz] / 2,
                             max_hit_dice[sz]);
    }

    // Recover all your spell slots
    level_t caster_level = _caster_level(*pc);
    for (size_t i = 0; i < 9; i++) {
        pc->base.info.spell_slots[i] = _num_slots(caster_level, i + 1);
    }
}
