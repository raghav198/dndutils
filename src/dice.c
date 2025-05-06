#include "dice.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

int dice_errno = 0;

void dice_seed(void) {
    // Don't seed if we've already seeded
    static bool seeded = false;
    if (seeded)
        return;
    seeded = true;

    // Try to get a seed from /dev/urandom
    int rand_fd = open("/dev/urandom", O_RDONLY);
    if (rand_fd < 0) {
        // Fall back to system time if necessary
        srand(time(0));
        return;
    }

    unsigned int seed;
    read(rand_fd, &seed, sizeof(unsigned int));
    srand(seed);
    close(rand_fd);
}

static int roll_comparator(const void *a, const void *b) {
    roll_result_t result_a = *(roll_result_t *)a;
    roll_result_t result_b = *(roll_result_t *)b;
    if (result_a < result_b)
        return -1;
    if (result_a > result_b)
        return 1;
    return 0;
}

roll_result_t dice_perform_roll(size_t count, size_t sides,
                                enum roll_aggregation_policy policy,
                                size_t policy_value) {
    dice_errno = 0;
    size_t results[count];

    // Perform the rolls
    for (size_t i = 0; i < count; i++) {
        results[i] = rand() % sides + 1;
    }

    // Debug
    fprintf(stderr, "Raw roll: ");
    for (size_t i = 0; i < count; i++)
        fprintf(stderr, "%zu ", results[i]);
    fprintf(stderr, "\n");

    // Aggregate
    switch (policy) {
    case KEEP_HIGH:
    case KEEP_LOW: {
        // Can't keep top three of only two rolls
        if (policy_value > count) {
            dice_errno = EROLL_INVAL;
            return -1;
        }
        qsort(results, count, sizeof(roll_result_t), roll_comparator);

        // Add up top/bottom N
        roll_result_t result = 0;
        for (size_t i = 0; i < policy_value; i++) {
            result += results[policy == KEEP_LOW ? i : (count - i - 1)];
        }
        return result;
    }
    case COUNT_ABOVE:
    case COUNT_BELOW: {
        // Filter and count
        roll_result_t result = 0;
        for (size_t i = 0; i < count; i++) {
            if ((policy == COUNT_ABOVE && results[i] > policy_value) ||
                (policy == COUNT_BELOW && results[i] < policy_value)) {
                result++;
            }
        }
        return result;
    }
    case SUM: {
        // Just add them up
        roll_result_t result = 0;
        for (size_t i = 0; i < count; i++) {
            result += results[i];
        }
        return result + policy_value;
    }
    }
    dice_errno = EROLL_INVAL;
    return -1;
}

static void _skip_whitespace(const char *str, size_t *index) {
    while (isspace(str[*index])) {
        (*index)++;
    }
}

static size_t _parse_int(const char *str, size_t *index) {
    _skip_whitespace(str, index);
    size_t value = 0;
    while (isdigit(str[*index])) {
        value *= 10;
        value += str[(*index)++] - '0';
    }
    return value;
}

static bool _has_int(const char *str, size_t index) {
    return isdigit(str[index]);
}

// Evaluate something like NdM(agg n)?
roll_result_t _evaluate_single_roll(const char *str, size_t *index) {
    if (!_has_int(str, *index)) {
        dice_errno = EROLL_EMPTY;
        return -1;
    }

    dice_errno = 0;

    size_t count = _parse_int(str, index);

    // If there's no dice involved, just return the number
    _skip_whitespace(str, index);
    if (str[*index] != 'd' && str[*index != 'D'])
        return count;

    (*index)++;
    size_t sides = _parse_int(str, index);

    // Check for an explicit aggregation policy
    _skip_whitespace(str, index);
    if (!strncasecmp(str + *index, "kh", 2)) {
        *index += 2;
        size_t policy_value = _parse_int(str, index);
        return dice_perform_roll(count, sides, KEEP_HIGH, policy_value);
    }
    if (!strncasecmp(str + *index, "kl", 2)) {
        *index += 2;
        size_t policy_value = _parse_int(str, index);
        return dice_perform_roll(count, sides, KEEP_LOW, policy_value);
    }
    if (!strncasecmp(str + *index, "<", 1)) {
        (*index)++;
        size_t policy_value = _parse_int(str, index);
        return dice_perform_roll(count, sides, COUNT_BELOW, policy_value);
    }
    if (!strncasecmp(str + *index, ">", 1)) {
        (*index)++;
        size_t policy_value = _parse_int(str, index);
        return dice_perform_roll(count, sides, COUNT_ABOVE, policy_value);
    }

    // If there is none, fall back to SUM
    return dice_perform_roll(count, sides, SUM, 0);
}

roll_result_t dice_evaluate_roll(const char *str, const char **endptr) {
    size_t index = 0;
    _skip_whitespace(str, &index);

    dice_errno = 0;

    roll_result_t result = 0;
    while (str[index] != '\0') {
        // Start by grabbing the first roll in the sequence
        result += _evaluate_single_roll(str, &index);
        if (dice_errno) {
            return -1;
        }

        _skip_whitespace(str, &index);
        if (str[index] != '+') {
            break;
        }
        index++;
    }

    if (endptr != NULL) {
        *endptr = str + index;
    }
    return result;
}
