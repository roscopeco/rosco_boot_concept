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

#ifndef __BLIT_RECTS_H
#define __BLIT_RECTS_H

#include <stdint.h>

typedef struct {
    uint16_t        start;
    uint16_t        words;
    uint16_t        value;
} BlitLinePart;

typedef struct {
    BlitLinePart  left;
    BlitLinePart  main;
    BlitLinePart  right;
} BlitLines;

// Calculate line values for blitter-filled rectangles in 4bpp modes.
//
// Splits rects into three - a left, main and right, for 
// rectangles that aren't aligned. Not all may be used - a width
// value of 0 indicates an unused rect (in which case, other values
// for that rect are undefined).
//
void blit_lines(uint16_t x, uint16_t w, BlitLines *result);

#endif