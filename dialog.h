#ifndef __DIALOG_H
#define __DIALOG_H

#include "window.h"
#include "backend.h"

// Horizontal padding for dialogs
#define DIALOG_ICON_WIDTH   16
#define DIALOG_ICON_HEIGHT  16

#if (VIEW_HRES == 320) || (VIEW_HRES == 424)
#define DIALOG_ICON_H_PAD   4
#define DIALOG_H_PAD        (( CLIENT_H_PAD * 4 ))
#elif (VIEW_HRES == 640) || (VIEW_HRES == 848)
#define DIALOG_ICON_H_PAD   8
#define DIALOG_H_PAD        (( CLIENT_H_PAD * 8 ))
#else
#   error Invalid resolution, please configure in dialog.h
#endif

// Addiitonal lines of vertical padding between message and options
#define MESSAGE_PAD_LINES   0

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

void dialog_window_abort_retry_fail_init(
    Window* window,
    WindowType window_type,
    char *title,
    BACKEND_COLOR title_color,
    char *message,
    BACKEND_COLOR text_color,
    DialogIconType icon_type
);

#endif