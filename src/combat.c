#include "combat.h"
#include "dice.h"
#include "skills.h"

struct attack_result npc_make_attack(struct npc_sheet attacker, struct npc_sheet defender,
                     enum stat attacking_stat) {
    mod_t mod = npc_get_mod(attacker, NUM_SKILLS, attacking_stat);
    roll_result_t attack_roll = dice_perform_roll(1, 20, POLICY_SUM, 0);
    if (attack_roll == 20) 
        return (struct attack_result){.hit = true, .crit = true};
    if (attack_roll == 1)
        return (struct attack_result){.hit = false, .crit = true};
    return (struct attack_result){.hit = (attack_roll + mod) >= defender.AC, .crit = false};
}

void npc_apply_damage(struct npc_sheet *npc, size_t damage) {
    if (npc->HP <= damage) {
        npc->HP = 0;
        return;
    }
    npc->HP -= damage;
}

// EXAMPLES OF COMBAT ABILITIES

void combat_eldritch_blast(const struct npc_sheet *attacker, struct npc_sheet *defender) {
    // Get the number of eldritch blast bolts
    size_t num_bolts = 1;
    if (attacker->CR > 4)
        num_bolts++;
    if (attacker->CR > 10)
        num_bolts++;
    if (attacker->CR > 16)
        num_bolts++;

    // Make a number of attack rolls against the defender's AC
    // (assume they all target the same creature)
    for (size_t i = 0; i < num_bolts; i++) {
        struct attack_result result = npc_make_attack(*attacker, *defender, CHA);
        if (result.hit) {
            roll_result_t damage =
                dice_perform_roll((result.crit ? 2 : 1), 10, POLICY_SUM, 0);
            npc_apply_damage(defender, damage);
        }
    }
}

void combat_fireball(const struct npc_sheet *attacker, struct npc_sheet *defenders,
                     size_t num_defenders) {

    // Assume wizard casting
    size_t dc = npc_calculate_dc(*attacker, INT);
    for (size_t i = 0; i < num_defenders; i++) {
        // Ask each defender to make a dex save
        roll_result_t save = npc_make_save(defenders[i], DEX);
        roll_result_t damage = dice_perform_roll(8, 6, POLICY_SUM, 0);
        // Halve the damage on a successful save
        if (save >= dc)
            damage /= 2;
        npc_apply_damage(&defenders[i], damage);
    }
}
void combat_longsword_strike(const struct npc_sheet *attacker,
                             struct npc_sheet *defender) {
    // Much easier: just make an attack roll
    struct attack_result result = npc_make_attack(*attacker, *defender, STR);
    if (result.hit) {
        roll_result_t damage =
            dice_perform_roll((result.crit ? 2 : 1), 8, POLICY_SUM, 0);
        npc_apply_damage(defender, damage);
    }
}
