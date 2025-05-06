#ifndef DICE_H_
#define DICE_H_

#include <stddef.h>

extern int dice_errno;

#define EROLL_INVAL 1
#define EROLL_EMPTY 2

typedef size_t roll_result_t;

enum roll_aggregation_policy {
    SUM,
    KEEP_HIGH,
    KEEP_LOW,
    COUNT_ABOVE,
    COUNT_BELOW
};

void dice_seed(void);
roll_result_t dice_perform_roll(size_t count, size_t sides,
                                enum roll_aggregation_policy policy,
                                size_t policy_value);
roll_result_t dice_evaluate_roll(const char *str, const char **endptr);

#endif // DICE_H_
