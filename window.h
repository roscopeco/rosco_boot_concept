#ifndef __WINDOW_H
#define __WINDOW_H

#include <stdbool.h>

#include "view.h"
#include "backend.h"
#include "model.h"

#define CLIENT_H_PAD        2

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

    WindowType          window_type;
    char*               title;
    char*               subtitle;

    WindowModel*        model;

    BACKEND_FONT_COOKIE regular_font;
    BACKEND_FONT_COOKIE small_font;

    WindowFunc          get_client_width_func;
    WindowFunc          get_client_height_func;
    WindowFunc          paint_client_area;

    EventFunc           dispatch_event_func;
} Window;

void window_paint(Window *window);

#endif
