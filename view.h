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

#define VIEW_HRES   640
#define VIEW_VRES   480

#define CENTER_ITEMS
#define HIGHLIGHT_SELECTION

#define MIN_PADDING     30              // at top and bottom of screen
#define BOX_WIDTH       ((VIEW_HRES/1.8))
#define LINE_HEIGHT     20
#define VERSION         "2.50.DEV"
#define SHADOW_OFFSET   8

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
