#ifndef __MODEL_H
#define __MODEL_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    /* Mutable - included in dirty check */
    uint8_t     selection;
    int         n_items;
    char**      items;
    uint8_t     timer_secs_left;

    /* Transient - not dirty checked */
    uint32_t    last_timer_ticks;

    /* Immutable - not dirty checked */
    uint32_t    mem_count;
    uint32_t    cpu;
    uint32_t    mhz;
} Model;

bool model_is_dirty(Model *current, Model *target);

#endif
