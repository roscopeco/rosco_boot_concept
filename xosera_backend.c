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
 * Xosera backend for concept boot screen
 * ------------------------------------------------------------
 */

#include "view.h"
#include "backend.h"

bool backend_init() {
    return false;
}

void backend_clear() {
}

void backend_set_color(
    __attribute__ ((unused)) uint8_t r, 
    __attribute__ ((unused)) uint8_t g,
    __attribute__ ((unused)) uint8_t b,
    __attribute__ ((unused)) uint8_t a
) {
}

void backend_draw_pixel(__attribute__ ((unused)) int x, __attribute__ ((unused)) int y) {
}

BACKEND_EVENT backend_poll_event() {
    return NONE;
}

uint32_t backend_get_ticks() {
    return 0;
}

void backend_draw_rect(__attribute__ ((unused)) Rect *rect) {
}

void backend_fill_rect(__attribute__ ((unused)) Rect *rect) {
}

void backend_present() {
}

void backend_done() {
}
