#include <string.h>

#include "view.h"
#include "backend.h"
#include "debug.h"

#if (VIEW_HRES == 320) 
#define BOX_WIDTH       ((VIEW_HRES/1.4))       /* Width of the box */
#elif (VIEW_HRES == 424)
#define BOX_WIDTH       ((VIEW_HRES/1.8))       /* Width of the box */
#elif (VIEW_HRES == 640)
#define BOX_WIDTH       ((VIEW_HRES/1.8))       /* Width of the box */
#elif (VIEW_HRES == 848)
#define BOX_WIDTH       ((VIEW_HRES/3.0))       /* Width of the box */
#else
#error Unknown resolution - add settings to main_menu.c
#endif

#define MAX_BOX_HEIGHT  ((VIEW_VRES-(MIN_PADDING*2)))

#define MODEL(window)       (( ((MainMenuModel*)window->model->window_data) ))

typedef struct {
    /* Mutable - included in dirty check */
    uint8_t     selection;
    int         n_items;
    char**      items;
    uint8_t     timer_secs_left;

    /* Transient - not dirty checked */
    uint32_t    last_4t_timer_ticks;
    uint32_t    last_1s_timer_ticks;

    /* Static - not dirty checked */
    BACKEND_FONT_COOKIE small_font;
} MainMenuModel;

#ifdef ENABLE_TIMER
static char secs_buf[2];
#endif

static WindowModel window_model;
static MainMenuModel main_menu_model;

static int get_window_width(__attribute__((unused)) Window *window) {
    return BOX_WIDTH;
}

static int calc_window_height(Window *window) {
    MainMenuModel *model = MODEL(window);

    int main_box_height = LINE_HEIGHT * model->n_items;

    if (main_box_height > MAX_BOX_HEIGHT) {
        model->n_items = MAX_BOX_HEIGHT / LINE_HEIGHT - 1;
        main_box_height = LINE_HEIGHT * (model->n_items + 1);
        debugf("WARN: Adjusted n_items to %d for height\n", model->n_items);
    }

    return main_box_height;
}

static int paint_client_area(Window *window) {
        // Selection bar
        Rect selection_rect;
        selection_rect.x = window->main_box.x + 1;
        selection_rect.y = window->main_box.y + window->main_box_header.h + (LINE_HEIGHT * MODEL(window)->selection);
        selection_rect.w = window->main_box.w - 2;
        selection_rect.h = LINE_HEIGHT - 1;  // -1 so as not to overdraw border on last item!
        backend_set_color(COLOR_SELECTION_BAR);
#ifdef BLIT_DEBUG
        printf("SELECTION    ::::::: ");
#endif   
        backend_fill_rect(&selection_rect);

        // Items text
        backend_set_color(COLOR_ITEM_TEXT);
        int y = CLIENT_AREA_Y(window);

        for (int i = 0; i < MODEL(window)->n_items; i++) {
#           ifdef CENTER_ITEMS
            int x = CLIENT_AREA_X(window) + (CLIENT_AREA_W(window) / 2) - (strlen(MODEL(window)->items[i]) * FONT_WIDTH / 2);
            #else
            int x = view->main_box_header.x + 4;
#           endif

#           ifdef HIGHLIGHT_SELECTION
            if (i == MODEL(window)->selection) {
                backend_set_color(COLOR_ITEM_HIGHLIGHT_TEXT);
            }
#           endif

            backend_text_write(MODEL(window)->items[i], x, y, window->font, FONT_WIDTH, FONT_HEIGHT);

#           ifdef HIGHLIGHT_SELECTION
            if (i == MODEL(window)->selection) {
                backend_set_color(COLOR_ITEM_TEXT);
            }
#           endif

            y += LINE_HEIGHT;
        }

#   ifdef ENABLE_TIMER
    // ticks remaining
    if (MODEL(window)->timer_secs_left) {
        secs_buf[0] = MODEL(window)->timer_secs_left;
        backend_set_color(COLOR_YELLOW);
        backend_text_write(secs_buf, selection_rect.x + selection_rect.w - 12, selection_rect.y + 2, MODEL(window)->small_font, NUM_FONT_WIDTH, NUM_FONT_HEIGHT);
    }
#   endif


    return 0;
}

static bool update_timers(__attribute__((unused)) Window *window) {
#ifdef ENABLE_TIMER
    uint32_t now = backend_get_ticks();
    MainMenuModel *model = MODEL(window);

    // Do 1s countdown timer, unless it's already expired
    if (model->timer_secs_left) {
        if (model->last_1s_timer_ticks == 0) {
            // first run, just set it up...
            // (or we've hit a rare actual zero rollover, whatever...)
            model->last_1s_timer_ticks = now;
            window->model->is_dirty = true;
        } else {
            uint32_t elapsed = now - model->last_1s_timer_ticks;

            // 1sec interval timer
            if (elapsed >= 100) {
                window->model->is_dirty = true;
                model->last_1s_timer_ticks = backend_get_ticks();
                model->timer_secs_left--;
            }

            if (!model->timer_secs_left) {
                // expired - return true.
                window->model->is_dirty = true;
                return true;
            }
        }
    }
#endif

    return false;
}


static bool dispatch_event_func(Window *window, BACKEND_EVENT event) {
    MainMenuModel *menu_model = MODEL(window);

    // do first, so keypress can set zero and kill timer...
    if (update_timers(window)) {
        debugf("Timer expired; default choice: %d\n", menu_model->selection);
        return false;
    };

    switch (event) {
    case UP:
        menu_model->timer_secs_left = 0;

        if (menu_model->selection == 0) {
            menu_model->selection = menu_model->n_items - 1;
        } else {
            menu_model->selection--;
        }

        window->model->is_dirty = true;

        return true;
    case DOWN:
        menu_model->timer_secs_left = 0;

        if (menu_model->selection == menu_model->n_items - 1) {
            menu_model->selection = 0;
        } else {
            menu_model->selection++;
        }

        window->model->is_dirty = true;

        return true;
#ifndef __ROSCO_M68K_ROM__
    case QUIT:
        return false;
#endif
    default:
        return true;
    }
}

void main_menu_window_init(
    Window* window,
    int n_items, 
    char **items, 
    BACKEND_FONT_COOKIE regular_font, 
    BACKEND_FONT_COOKIE small_font
) {
    main_menu_model.items = items;
    main_menu_model.n_items = n_items;
    main_menu_model.timer_secs_left = 5;
    main_menu_model.small_font = small_font;

    window_model.is_dirty = true;
    window_model.window_data = &main_menu_model;

    window->active = true;
    window->window_type = WINDOW_TYPE_NORMAL;

    window->model = &window_model;

    window->get_client_height_func = calc_window_height;
    window->get_client_width_func = get_window_width;
    window->paint_client_area = paint_client_area;
    window->dispatch_event_func = dispatch_event_func;

    window->title = BOX_TITLE;
    window->subtitle = VERSION;
    window->font = regular_font;
    window->has_shadow = true;

    window->background_color = COLOR_WINDOW_BACKGROUND;
    window->border_color = COLOR_BLACK;
    window->shadow_color = COLOR_BACKGROUND_SHADOW;
    window->title_color = COLOR_WHITE;
    window->subtitle_color = COLOR_YELLOW;
}
