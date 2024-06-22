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
 * Blitter rectangle calcs for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdint.h>

#include "blit_rects.h"

void blit_lines(uint16_t x, uint16_t w, BlitLines *result) {
    unsigned int x_end = x + w - 1;

    unsigned int x_div2 = x >> 2;
    unsigned int x_div2_mul2 = x_div2 << 2;
    unsigned int xend_div2_mul2 = x_end >> 2 << 2;

    unsigned int x_bound = x - x_div2_mul2;
    unsigned int x_end_bound = x_end - xend_div2_mul2;

    // main rect
    unsigned int left_rect_x, main_rect_x, right_rect_x;
    unsigned int left_rect_w, main_rect_w, right_rect_w;
    unsigned int main_rect_x_end;
    unsigned int left_rect_const = 0, main_rect_const = 0, right_rect_const = 0;

    if (x_bound == 0) {
    // start is on a boundary
        main_rect_x = x_div2_mul2;
    } else {
        main_rect_x = x_div2 + 4;
    }

    if (x_end_bound == 3) {
        // end is on a boundary
        main_rect_x_end = xend_div2_mul2 + 3;
    } else {
        main_rect_x_end = xend_div2_mul2 - 1;
    }

    main_rect_w = main_rect_x_end - main_rect_x + 1;

    if (main_rect_w < 4) {
        main_rect_x = 0;
//        main_rect_x_end = 0;
        main_rect_w = 0;
    } else {
        main_rect_const = 0xFFFF;
    }

    // left rect
    if (x_bound == 0) {
        // start is on a boundary - no left rect
        left_rect_x = 0;
        left_rect_w = 0;
    } else {
        left_rect_x = x_div2;
        left_rect_w = 4;

        if (x_bound == 1) {
            left_rect_const = 0x0FFF;
        } else if (x_bound == 2) {
            left_rect_const = 0x00FF;
        } else if (x_bound == 3) {
            left_rect_const = 0x000F;
        } else {
            // WTAF?
        }
    }

    // right rect
    if (x_end_bound == 3) {
        // end is on a boundary - no right rect
        right_rect_x = 0;
        right_rect_w = 0;
    } else {
        right_rect_x = xend_div2_mul2;
        right_rect_w = 4;

        if (x_end_bound == 0) {
            right_rect_const = 0xF000;
        } else if (x_bound == 1) {
            right_rect_const = 0xFF00;
        } else if (x_bound == 2) {
            right_rect_const = 0xFFF0;
        } else { 
            // WTAF?
        }
    }

    result->left.start = left_rect_x / 4;
    result->left.words = left_rect_w / 4;
    result->left.value = left_rect_const;

    result->main.start = main_rect_x / 4;
    result->main.words = main_rect_w / 4;
    result->main.value = main_rect_const;

    result->right.start = right_rect_x / 4;
    result->right.words = right_rect_w / 4;
    result->right.value = right_rect_const;
}