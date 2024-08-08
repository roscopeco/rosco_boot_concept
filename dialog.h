#ifndef __DIALOG_H
#define __DIALOG_H

#include "window.h"
#include "backend.h"

void dialog_window_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,
    const char **options,
    const int n_options
);

void dialog_window_ok_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color
);

void dialog_window_yes_no_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color
);


#endif