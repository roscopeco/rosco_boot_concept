#include <string.h>
#include "view.h"
#include "window.h"
#include "dialog.h"
#include "graphics.h"
#include "graphics_utils.h"

// Horizontal padding for dialogs
#define DIALOG_H_PAD        (( CLIENT_H_PAD * 8 ))

// Addiitonal lines of vertical padding between message and options
#define MESSAGE_PAD_LINES   0

#define MODEL(window)       (( ((DialogModel*)window->model->window_data) ))

typedef struct {
    /* User */
    char*           message;
    const char**    options;
    int             n_options;
    int             selection;
    BACKEND_COLOR   title_color;
    BACKEND_COLOR   text_color;

    /* Computed */
    int             message_width_chars;
    int             message_height_lines;
} DialogModel;

static const char* opts_ok_strings[] = {
    "Ok"
};

static const int opts_ok_count = 1;

static const char* opts_yes_no_strings[] = {
    "Yes",
    "No"
};

static const int opts_yes_no_count = 2;

/* Only support one at a time for now... */
static WindowModel window_model;
static DialogModel dialog_model;

static int get_window_width(Window *window) {
    return MODEL(window)->message_width_chars * FONT_WIDTH + DIALOG_H_PAD;
}

static int calc_window_height(__attribute__((unused)) Window *window) {
    return (MODEL(window)->message_height_lines + MESSAGE_PAD_LINES + MODEL(window)->n_options) * LINE_HEIGHT + LINE_PAD;
}

static int paint_client_area(Window *window) {    
    backend_set_color(MODEL(window)->text_color);
    int y = CLIENT_AREA_Y(window);
    int options_y = CLIENT_AREA_Y(window) + (LINE_HEIGHT * (MODEL(window)->message_height_lines + MESSAGE_PAD_LINES));
    int x = CLIENT_AREA_X(window) + (CLIENT_AREA_W(window) / 2) - ((MODEL(window)->message_width_chars * FONT_WIDTH + (DIALOG_H_PAD / 2)) / 2);
    
    gfx_render_text(MODEL(window)->message, x, y, window->font, FONT_WIDTH, FONT_HEIGHT);

    // Selection bar
    Rect temp_rect;
    temp_rect.x = window->main_box.x + 1;
    temp_rect.y = options_y + (LINE_HEIGHT * MODEL(window)->selection);
    temp_rect.w = CLIENT_AREA_W(window) - 2;
    temp_rect.h = LINE_HEIGHT - 1;  // -1 so as not to overdraw border on last item!
    backend_set_color(COLOR_SELECTION_BAR);
    backend_fill_rect(&temp_rect);

    // Divider
    temp_rect.y = options_y;
    temp_rect.h = 1;
    backend_set_color(COLOR_BLACK);
    backend_fill_rect(&temp_rect);

    // Items text
    backend_set_color(COLOR_ITEM_TEXT);
    options_y += ((LINE_HEIGHT - FONT_HEIGHT) / 2);

    for (int i = 0; i < MODEL(window)->n_options; i++) {
#       ifdef CENTER_ITEMS
        int x = CLIENT_AREA_X(window) + (CLIENT_AREA_W(window) / 2) - (strlen(MODEL(window)->options[i]) * FONT_WIDTH / 2);
        #else
        int x = view->main_box_header.x + 4;
#       endif

#       ifdef HIGHLIGHT_SELECTION
        if (i == MODEL(window)->selection) {
            backend_set_color(COLOR_ITEM_HIGHLIGHT_TEXT);
        }
#       endif

        backend_text_write(MODEL(window)->options[i], x, options_y, window->font, FONT_WIDTH, FONT_HEIGHT);

#       ifdef HIGHLIGHT_SELECTION
        if (i == MODEL(window)->selection) {
            backend_set_color(COLOR_ITEM_TEXT);
        }
#       endif

        options_y += LINE_HEIGHT;
    }

    return 0;
}

static bool dispatch_event_func(__attribute__((unused)) Window *window, BACKEND_EVENT event) {
    DialogModel *dialog_model = MODEL(window);

    switch (event) {
    case UP:
        if (dialog_model->selection == 0) {
            dialog_model->selection = dialog_model->n_options - 1;
        } else {
            dialog_model->selection--;
        }

        window->model->is_dirty = true;

        return true;
    case DOWN:
        if (dialog_model->selection == dialog_model->n_options - 1) {
            dialog_model->selection = 0;
        } else {
            dialog_model->selection++;
        }

        window->model->is_dirty = true;

        return true;

#ifndef __ROSCO_M68K_ROM__
    case QUIT:
#endif
        return false;
    default:
        return true;
    }
}

void dialog_window_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,
    const char **options,
    const int n_options
) {
    dialog_model.message = message;
    dialog_model.title_color = title_color;
    dialog_model.text_color = text_color;

    if (!gfx_measure_text(message, &dialog_model.message_width_chars, &dialog_model.message_height_lines)) {
        dialog_model.message_width_chars = strlen(message);
        dialog_model.message_height_lines = 1;
    }

    dialog_model.options = options;
    dialog_model.n_options = n_options;

    window_model.is_dirty = true;
    window_model.window_data = &dialog_model;

    window->active = true;
    window->window_type = window_type;

    window->model = &window_model;    

    window->get_client_height_func = calc_window_height;
    window->get_client_width_func = get_window_width;
    window->paint_client_area = paint_client_area;
    window->dispatch_event_func = dispatch_event_func;

    window->title = title;
    window->subtitle = NULL;
    window->font = view_get_regular_font();
    window->has_shadow = false;

    window->background_color = COLOR_DIALOG_BACKGROUND;
    window->border_color = COLOR_BLACK;
    window->shadow_color = COLOR_BACKGROUND_SHADOW;
    window->title_color = title_color;
    window->subtitle_color = COLOR_YELLOW;
}

void dialog_window_ok_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color
) {
    dialog_window_init(
        window,
        window_type,
        title,
        title_color,
        message,
        text_color,
        opts_ok_strings,
        opts_ok_count
    );
}

void dialog_window_yes_no_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color
) {
    dialog_window_init(
        window,
        window_type,
        title,
        title_color,
        message,
        text_color,
        opts_yes_no_strings,
        opts_yes_no_count
    );
}