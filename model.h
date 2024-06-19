/*
 *------------------------------------------------------------
 *                                  ___ ___ _
 *  ___ ___ ___ ___ ___       _____|  _| . | |_
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_|
 *                     |_____|          firmware
 * ------------------------------------------------------------
 * Copyright (c)2024 The rosco_m68k Open Source Project
 * See top-level LICENSE.md for licence information.
 *
 * Model for concept boot screen
 * ------------------------------------------------------------
 */

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
