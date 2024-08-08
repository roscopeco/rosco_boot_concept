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
 * Main for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdio.h>

#include "model.h"
#include "view.h"
#include "controller.h"

#include "main_menu.h"
#include "dialog.h"

#ifdef ENABLE_ANIM
#include "animation.h"
#ifdef SHOW_TEST_ANIM
#include "test_anim.h"
#endif
#endif

#include "backend.h"

static int n_menu_items = 4;
static char* menu_items[] = {
    "Program loader",
    "MicroPython",
    "System configuration",
    "Memory check & diagnostic",
};

static Model model;
static View view;

#ifdef ENABLE_ANIM
#ifdef SHOW_TEST_ANIM
static TestAnimation test_animation;
static TestAnimation test_animation_2;
#endif
#endif

int main(void) {
    if (!backend_init()) {
        printf("Backend init failed\n");
        return 2;
    }

    model.mem_count = backend_get_memsize();
    model.cpu = backend_get_cpu();
    model.mhz = backend_get_cpu_mhz();

#ifdef ENABLE_ANIM
#ifdef SHOW_TEST_ANIM
    test_animation.animation.tick = test_anim_tick;
    test_animation.animation.paint = test_anim_paint;
    test_animation_2.animation.tick = test_anim_tick;
    test_animation_2.animation.paint = test_anim_paint;
    test_animation_2.next_y = 40;

    list_insert_after((ListNode*)&test_animation, &model.animations_front);
    list_insert_after((ListNode*)&test_animation_2, &model.animations_back);
#endif
#endif

    view_init(&view, &model);

    main_menu_window_init(
        &view.windows[0], 
        n_menu_items, 
        menu_items, 
        view_get_regular_font(), 
        view_get_small_font()
    );

    dialog_window_ok_init(
        &view.windows[1],
        WINDOW_TYPE_DIALOG,
        "Dire warning!",
        COLOR_ITEM_HIGHLIGHT_TEXT,
        "Please don't do that again.",
        COLOR_ITEM_TEXT,
        DIALOG_ICON_WARNING
    );

    while (control(&view)) {
        view_repaint(&view, false);
    }

    backend_done();
}

