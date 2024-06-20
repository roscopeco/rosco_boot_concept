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
 * View for concept boot screen
 * ------------------------------------------------------------
 */

#ifndef __VIEW_H
#define __VIEW_H

#include <stdint.h>
#include <stdbool.h>
#include "model.h"

#define VIEW_HRES   320
#define VIEW_VRES   240
#define VERSION     "2.50.DEV"

//#define CENTER_ITEMS
#define HIGHLIGHT_SELECTION
#define FONT_8X8

#if VIEW_HRES == 320
#include "topaz_font.h"
#define BOX_WIDTH       ((VIEW_HRES/1.4))
#elif VIEW_HRES == 640
#include "main_font.h"
#define BOX_WIDTH       ((VIEW_HRES/1.8))
#else
#error Unknown resolution - add settings to view.h
#endif
#include "num_font.h"

#define MIN_PADDING     ((LINE_HEIGHT * 1.5))       // at top and bottom of screen
#define LINE_HEIGHT     ((FONT_HEIGHT + (FONT_HEIGHT / 4)))
#define LINE_PAD        (((FONT_HEIGHT / 8)))
#define SHADOW_OFFSET   ((VIEW_HRES / 80))

typedef struct {
    int x, y, w, h;
} Rect;

typedef struct {
    Rect    main_box;
    Rect    main_box_header;
    Rect    right_shadow;
    Rect    bottom_shadow;
    Model*  model;
} View;

void view_init(View *view, Model *model);
void view_recompute_size(View *view, Model *model);
void view_repaint(View *view, bool force);

#endif
