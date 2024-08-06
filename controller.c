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
 * Controller for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdio.h>
#include <stdbool.h>

#include "view.h"
#include "model.h"
#include "backend.h"

#include "config.h"

#ifdef ENABLE_ANIM
static void tick_anims(Model *model, uint32_t now) {
    Animation* current_front = (Animation*)model->animations_front.next;
    Animation* current_back = (Animation*)model->animations_back.next;

    if (current_front || current_back) {
        if (model->last_4t_timer_ticks == 0) {
            // first run, just set it up...
            // (or we've hit a rare actual zero rollover, whatever...)
            model->last_4t_timer_ticks = now;
        } else {
            uint32_t elapsed = now - model->last_4t_timer_ticks;

            // 4tick interval timer
            if (elapsed >= 4) {

                while (current_front || current_back) {
                    if (current_front) {
                        if (!current_front->tick(now, current_front)) {
                            list_delete(&current_front->node);
                            model->anim_list_dirty = true;
                        }
                        current_front = (Animation*)current_front->node.next;
                    }
                    if (current_back) {
                        if (!current_back->tick(now, current_back)) {
                            list_delete(&current_back->node);
                            model->anim_list_dirty = true;
                        }
                        current_back = (Animation*)current_back->node.next;
                    }
                }

                model->last_4t_timer_ticks = backend_get_ticks();
            }
        }
    }
}
#else
#define tick_anims(...)
#endif

bool control(View *view) {
    // Do animations first, unless there aren't any active...
    tick_anims(view->model, backend_get_ticks());

    BACKEND_EVENT event = backend_poll_event();
    Window *active_window = view_get_active_window(view);

    if (active_window) {
        return active_window->dispatch_event_func(active_window, event);
    } else {
        printf("WARN: No active window!\n");
    }

    return true;    
}
