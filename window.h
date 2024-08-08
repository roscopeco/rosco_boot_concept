#ifndef __WINDOW_H
#define __WINDOW_H

#include <stdbool.h>

#include "view.h"
#include "backend.h"
#include "model.h"

#define CLIENT_H_PAD        2

#define CLIENT_AREA_X(window)       (( window->main_box.x + CLIENT_H_PAD ))
#define CLIENT_AREA_Y(window)       (( window->main_box_header.y + window->main_box_header.h + LINE_PAD ))
#define CLIENT_AREA_W(window)       (( window->main_box.w ))

typedef enum {
    WINDOW_TYPE_NORMAL      = 0,
    WINDOW_TYPE_DIALOG,
} WindowType;

typedef struct _Window Window;

typedef int     (*WindowFunc)(struct _Window *window);
typedef bool    (*EventFunc)(struct _Window *window, BACKEND_EVENT event);

typedef struct _Window {
    bool                active;

    Rect                main_box;
    Rect                main_box_header;
    Rect                right_shadow;
    Rect                bottom_shadow;

    bool                has_shadow;

    WindowType          window_type;
    char*               title;
    char*               subtitle;

    WindowModel*        model;

    BACKEND_FONT_COOKIE font;

    BACKEND_COLOR       background_color;
    BACKEND_COLOR       border_color;
    BACKEND_COLOR       shadow_color;
    BACKEND_COLOR       title_color;
    BACKEND_COLOR       subtitle_color;

    WindowFunc          get_client_width_func;
    WindowFunc          get_client_height_func;
    WindowFunc          paint_client_area;

    EventFunc           dispatch_event_func;
} Window;

void window_paint(Window *window);

#endif