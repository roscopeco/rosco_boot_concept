#ifndef __DIALOG_H
#define __DIALOG_H

#include "window.h"
#include "backend.h"

void dialog_window_init(
    Window* window,
    WindowType window_type,
    char *title,
    char *message,
     BACKEND_FONT_COOKIE font 
);


#endif