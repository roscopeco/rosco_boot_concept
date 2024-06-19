#include <stdio.h>
#include <stdbool.h>

#include "model.h"
#include "backend.h"

static bool update_timer(Model *model) {
    if (model->timer_secs_left) {
        uint32_t now = backend_get_ticks();

        if (model->last_timer_ticks == 0) {
            // first run, just set it up...
            // (or we've hit a rare actual zero rollover, whatever...)
            model->last_timer_ticks = now;
        } else {
            uint32_t elapsed = now - model->last_timer_ticks;

            if (elapsed >= 100) {
                model->last_timer_ticks = backend_get_ticks();
                model->timer_secs_left--;

                return model->timer_secs_left == 0;
            }
        }
    }

    return false;
}

bool control(Model *model) {
    // do first, so keypress can set zero and kill timer...
    if (update_timer(model)) {
        printf("Timer expired; default choice: %d\n", model->selection);
        return false;
    };


    BACKEND_EVENT event = backend_poll_event();
    switch (event) {
    case UP:
        model->timer_secs_left = 0;

        if (model->selection == 0) {
            model->selection = model->n_items - 1;
        } else {
            model->selection--;
        }

        return true;
    case DOWN:
        model->timer_secs_left = 0;

        if (model->selection == model->n_items - 1) {
            model->selection = 0;
        } else {
            model->selection++;
        }

        return true;
#ifndef __ROSCO_M68K
    case QUIT:
        return false;
#endif
    default:
        return true;
    }
}
