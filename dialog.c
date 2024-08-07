#include <string.h>
#include "view.h"
#include "window.h"
#include "dialog.h"

#define MODEL(window)       (( ((DialogModel*)window->model->window_data) ))

typedef struct {
    char*           message;
    BACKEND_COLOR   title_color;
    BACKEND_COLOR   text_color;
} DialogModel;

static WindowModel window_model;
static DialogModel dialog_model;

static int get_window_width(Window *window) {
    return strlen(MODEL(window)->message) * FONT_WIDTH + CLIENT_H_PAD;
}

static int calc_window_height(__attribute__((unused)) Window *window) {
    return LINE_HEIGHT;
}

static int paint_client_area(Window *window) {    
    backend_set_color(MODEL(window)->text_color);
    int y = CLIENT_AREA_Y(window);
    int x = CLIENT_AREA_X(window) + (CLIENT_AREA_W(window) / 2) - (strlen(MODEL(window)->message) * FONT_WIDTH / 2);
    
    backend_text_write(MODEL(window)->message, x, y, window->font, FONT_WIDTH, FONT_HEIGHT);

    return 0;
}

static bool dispatch_event_func(__attribute__((unused)) Window *window, BACKEND_EVENT event) {
    switch (event) {
    case UP:
    case DOWN:
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
    BACKEND_FONT_COOKIE font 
) {
    dialog_model.message = message;
    dialog_model.title_color = title_color;
    dialog_model.text_color = text_color;

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
    window->font = font;
}


