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
 * Text rendering for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdint.h>

#include "backend.h"

void text_write(const char *str, int x, int y, const uint8_t *font, int font_width, int font_height) {
    unsigned char c;

    while ((c = *str++)) {
        const uint8_t *font_ptr = font + (c * font_height);

        for (int dy = 0; dy < font_height; dy++) {
            for (int dx = 0; dx < font_width; dx++) {
                if ((*font_ptr & (1 << (font_width-1-dx))) != 0) {
                    backend_draw_pixel(x+dx, y+dy);
                }
            
            }

            font_ptr++;
        }

        x += font_width;
    }
}
