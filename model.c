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

#include <string.h>
#include <stdbool.h>

#include "model.h"

bool model_is_dirty(Model *current, Model *target) {
    if (!current || !target) {
        return current != target;
    } else {
        bool dirty = current->selection != target->selection 
            || current->timer_secs_left != target->timer_secs_left
            || current->n_items != target->n_items 
            || current->items != target->items;         // N.B. Not deep equality!

        memcpy(current, target, sizeof(Model));
        // // TODO just memcpy this now it's gotten bigger...
        // current->selection = target->selection;
        // current->n_items = target->n_items;
        // current->items = target->items;
        // current->mem_count = target->mem_count;
        // current->cpu = target->cpu;
        // current->mhz = target->mhz;

        return dirty;
    }
}