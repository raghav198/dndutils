#pragma once

#define DND_INCREASE_CLAMPED(val, amount, max)                                 \
    do {                                                                       \
        *(val) += (amount);                                                    \
        if (*(val) > (max))                                                    \
            *(val) = (max);                                                    \
    } while (0)
