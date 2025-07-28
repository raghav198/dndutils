#ifndef DICE_H_
#define DICE_H_

#include <stddef.h>

extern int dice_errno;

#define EROLL_INVAL 1
#define EROLL_EMPTY 2

typedef size_t roll_result_t;

enum dice_size { D4, D6, D8, D10, D12, D20, D100, NUM_SIZES };
size_t dice_get_sides(enum dice_size size);

enum roll_aggregation_policy {
    POLICY_SUM,
    POLICY_KEEP_HIGH,
    POLICY_KEEP_LOW,
    POLICY_COUNT_ABOVE,
    POLICY_COUNT_BELOW
};

void dice_seed(void);
roll_result_t dice_perform_roll(size_t count, size_t sides,
                                enum roll_aggregation_policy policy,
                                size_t policy_value);
roll_result_t dice_evaluate_roll(const char *str, const char **endptr);

#endif // DICE_H_
