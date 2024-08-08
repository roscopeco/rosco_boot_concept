#include <stdlib.h>
#include <string.h>

#include "view.h"
#include "backend.h"

static void window_recompute_size(Window *window) {
    int main_box_width = window->get_client_width_func(window) + CLIENT_H_PAD;
    int main_box_height = window->get_client_height_func(window);

    if (window->window_type == WINDOW_TYPE_NORMAL) {
        main_box_height += LINE_HEIGHT;        
    }

    window->main_box.x = VIEW_HRES / 2 - main_box_width / 2;
    window->main_box.y = VIEW_VRES / 2 - main_box_height / 2;
    window->main_box.w = main_box_width;
    window->main_box.h = main_box_height;

    window->main_box_header.x = window->main_box.x;
    window->main_box_header.y = window->main_box.y;
    window->main_box_header.w = window->main_box.w;

    if (window->window_type == WINDOW_TYPE_DIALOG) {
        window->main_box_header.h = 0;
    } else {
        window->main_box_header.h = LINE_HEIGHT;
    }

    if (window->has_shadow) {
        window->right_shadow.x = window->main_box.x + window->main_box.w;
        window->right_shadow.y = window->main_box.y + SHADOW_OFFSET;
        window->right_shadow.w = SHADOW_OFFSET;
        window->right_shadow.h = window->main_box.h;

        window->bottom_shadow.x = window->main_box.x + SHADOW_OFFSET;
        window->bottom_shadow.y = window->main_box.y + window->main_box.h;
        window->bottom_shadow.w = window->main_box.w;
        window->bottom_shadow.h = SHADOW_OFFSET;
    }
}

static void window_paint_chrome(Window *window) {
    // Main box
    backend_set_color(window->background_color);
    backend_fill_rect(&window->main_box);

    backend_set_color(window->border_color);

    // Title bar
    if (window->window_type == WINDOW_TYPE_NORMAL) {
        backend_fill_rect(&window->main_box_header);
    }

    // Border
    backend_draw_rect(&window->main_box);

    // Shadow
    if (window->has_shadow) {
        backend_set_color(window->shadow_color);
        backend_fill_rect(&window->right_shadow);
        backend_fill_rect(&window->bottom_shadow);
    }

    // Header text
    if (window->window_type != WINDOW_TYPE_DIALOG) {
        if (window->title != NULL) {
            backend_set_color(window->title_color);
            backend_text_write(window->title, window->main_box_header.x + 4, window->main_box_header.y + 2, window->font, FONT_WIDTH, FONT_HEIGHT);
        }

        if (window->subtitle != NULL) {
            backend_set_color(window->subtitle_color);
            backend_text_write(window->subtitle, window->main_box_header.x + window->main_box_header.w - 4 - (strlen(VERSION) * FONT_WIDTH), window->main_box_header.y + 2, window->font, FONT_WIDTH, FONT_HEIGHT);
        }
    }
}

void window_paint(Window *window) {    
    window_recompute_size(window);
    window_paint_chrome(window);
    window->paint_client_area(window);
    window->model->is_dirty = false;
}