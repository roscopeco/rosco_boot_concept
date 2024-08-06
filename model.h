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

#include "config.h"
#include "list.h"
#include "animation.h"

typedef struct {
#ifdef ENABLE_ANIM
    bool        anim_list_dirty;
    ListNode    animations_back;
    ListNode    animations_front;
#endif

    /* Immutable - not dirty checked */
    uint32_t    mem_count;
    uint32_t    cpu;
    uint32_t    mhz;

    /* Transient - not dirty checked */
    uint32_t    last_4t_timer_ticks;
    uint32_t    last_1s_timer_ticks;
} Model;

typedef struct {
    bool        is_dirty;
    void*       window_data;
} WindowModel;

#endif
