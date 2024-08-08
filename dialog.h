#ifndef __DIALOG_H
#define __DIALOG_H

#include "window.h"
#include "backend.h"

typedef enum {
    DIALOG_ICON_NONE        = 0,
    DIALOG_ICON_CHECK,
    DIALOG_ICON_WARNING,
    DIALOG_ICON_STOP,
} DialogIconType;

void dialog_window_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,    
    const char **options,
    const int n_options,
    DialogIconType icon_type
);

void dialog_window_ok_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,
    DialogIconType icon_type
);

void dialog_window_yes_no_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,
    DialogIconType icon_type
);


#endif