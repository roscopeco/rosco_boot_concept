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

#include <stdio.h>
#include <string.h>

#include "view.h"
#include "model.h"

#include "backend.h"

#include "debug.h"

#define CHAR_BUF_SIZE   16

#ifndef strnlen
// This is really there in newlib, but needs the right compiler opts...
// we'll just force the issue instead :D
size_t strnlen(const char *str, size_t n);
#endif

static char cpu_buffer[CHAR_BUF_SIZE];
static char mem_buffer[CHAR_BUF_SIZE];
static uint8_t cpu_buffer_len;
static uint8_t copyright_len;

static BACKEND_FONT_COOKIE regular_font;
static BACKEND_FONT_COOKIE small_font;

#ifdef TEST_PATTERN_DEBUG
extern bool use_ruler;
#endif

void view_init(View *view, Model *model) {
    view->model = model;

    snprintf(mem_buffer, CHAR_BUF_SIZE, "%dMB RAM", model->mem_count / 1048576);
    snprintf(cpu_buffer, CHAR_BUF_SIZE, "MC%d @ %dMHz", model->cpu, model->mhz);

    cpu_buffer_len = strnlen(cpu_buffer, CHAR_BUF_SIZE);
    copyright_len = strnlen(COPYRIGHT, COPYRIGHT_MAX);

    regular_font = backend_load_font(FONT, FONT_WIDTH, FONT_HEIGHT, 256);
    small_font = backend_load_font(NUM_FONT, NUM_FONT_WIDTH, NUM_FONT_HEIGHT, 6);
}

BACKEND_FONT_COOKIE view_get_regular_font() {
    return regular_font;
}

BACKEND_FONT_COOKIE view_get_small_font() {
    return small_font;
}

#ifdef __ROSCO_M68K_ROM__
#define debug_model_update(...)
#else
#ifdef MODEL_DEBUG
void debug_model_update(Model *model) {
    debugf("Model updated:\n");
    debugf("    selection: 0x%02x\n", model->selection);
    debugf("    secs_left: 0x%02x\n", model->timer_secs_left);
    debugf("    items_ptr: %p\n", model->items);
    debugf("    n_items  : %d\n", model->n_items);

#   ifdef ENABLE_ANIM
    debugf("  anim_dirty : %d\n", model->anim_list_dirty);
    debugf("  anim_front : %p\n", model->animations_front.next);
    debugf("   anim_back : %p\n", model->animations_back.next);
#   endif

    debugf("\n");
}
#else
#define debug_model_update(...)
#endif
#endif

#ifdef ENABLE_ANIM
static void paint_anim_layer(Animation *current_anim) {
    while (current_anim) {
        // Don't check dirty, always paint anims when painting.
        // **do** set dirty false though, in case the anim was what triggered this repaint...
        current_anim->paint(current_anim);
        current_anim->dirty = false;
        current_anim = (Animation*)current_anim->node.next;
    }
}

static bool anims_dirty(Animation *current_anim) {
    while (current_anim) {
        if (current_anim->dirty) {
            return true;
        }
        current_anim = (Animation*)current_anim->node.next;
    }

    return false;
}
#else
#define paint_anim_layer(...)
#endif

static bool is_dirty(View *view) {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (view->windows[i].active && view->windows[i].model->is_dirty) {
            return true;
        }
    }

#   ifdef ENABLE_ANIM    
    return view->model->anim_list_dirty
        || anims_dirty((Animation*)view->model->animations_back.next)
        || anims_dirty((Animation*)view->model->animations_front.next)
        ;
#   else
    return false;
#   endif
}

void view_repaint(View *view, bool force) {
    if (force || is_dirty(view)) {        
        debug_model_update(view->model);

        backend_set_color(COLOR_BACKGROUND);
        backend_clear();

        // Sysinfo header
        backend_set_color(COLOR_BACKGROUND_SHADOW);
        backend_text_write(mem_buffer, 6, 6, regular_font, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write(cpu_buffer, VIEW_HRES - (cpu_buffer_len * FONT_WIDTH) - 6, 6, regular_font, FONT_WIDTH, FONT_HEIGHT);

        // Copyright footer
        backend_text_write(COPYRIGHT, VIEW_HRES - (copyright_len * FONT_WIDTH) - 6, VIEW_VRES - LINE_HEIGHT, regular_font, FONT_WIDTH, FONT_HEIGHT);

        // Animations (back)
        paint_anim_layer((Animation*)view->model->animations_back.next);

        for (int i = 0; i < MAX_WINDOWS; i++) {
            Window *window = &view->windows[i];
            
            if (!window->active) {
                break;
            }

            window_paint(window);
        }

        // Animations (front)
        paint_anim_layer((Animation*)view->model->animations_front.next);

#       ifdef TEST_PATTERN_DEBUG
        use_ruler = true;
        Rect ruler = { .x = 0, .y = 30, .w = 20, .h = 2 };
        backend_set_color(COLOR_WHITE);
        backend_fill_rect(&ruler);
        
        use_ruler = false;
        Rect test1 = { .x = 0, .y = 32, .w = 4, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test1);

        Rect test2 = { .x = 0, .y = 35, .w = 5, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test2);

        Rect test3 = { .x = 0, .y = 38, .w = 6, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test3);

        Rect test4 = { .x = 0, .y = 41, .w = 7, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test4);

        Rect test5 = { .x = 0, .y = 44, .w = 8, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test5);

        Rect test6 = { .x = 1, .y = 47, .w = 8, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test6);

        Rect test7 = { .x = 2, .y = 50, .w = 8, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test7);

        Rect test8 = { .x = 0, .y = 53, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test8);

        Rect test9 = { .x = 1, .y = 56, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test9);

        Rect test10 = { .x = 2, .y = 59, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test10);

        Rect test11 = { .x = 3, .y = 62, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test11);

        Rect test12 = { .x = 4, .y = 65, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test12);

        Rect test13 = { .x = 5, .y = 68, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test13);

        Rect test14 = { .x = 6, .y = 71, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test14);

        Rect test15 = { .x = 7, .y = 74, .w = 1, .h = 2 };
        backend_set_color(COLOR_YELLOW);
        backend_fill_rect(&test15);

        backend_text_write("TEST[0,150]", 0, 150, regular_font, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write("TEST[1,170]", 1, 170, regular_font, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write("TEST[2,190]", 2, 190, regular_font, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write("TEST[3,210]", 3, 210, regular_font, FONT_WIDTH, FONT_HEIGHT);
        backend_text_write("TEST[4,230]", 4, 230, regular_font, FONT_WIDTH, FONT_HEIGHT);
#       endif

        backend_present();
    }
}

Window* view_get_active_window(View *view) {
    Window *active = NULL;

    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (view->windows[i].active) {
            active = &view->windows[i];
        } else {
            return active;
        }
    }

    // Should never be reached...
    return NULL;
}
