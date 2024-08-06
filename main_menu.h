#ifndef __MAIN_MENU_H
#define __MAIN_MENU_H

#include "window.h"
#include "backend.h"

void main_menu_window_init(
    Window* window,
    int n_items, 
    char **items, 
    BACKEND_FONT_COOKIE regular_font, 
    BACKEND_FONT_COOKIE small_font
);

#endif