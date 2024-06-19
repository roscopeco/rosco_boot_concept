#ifndef __BACKEND_H
#define __BACKEND_H

#include <stdint.h>
#include <stdbool.h>

#include "view.h"

typedef enum {
    NONE,
    QUIT,
    UP,
    DOWN
} BACKEND_EVENT;

bool backend_init();
void backend_clear();
void backend_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void backend_draw_pixel(int x, int y);
void backend_draw_rect(Rect *rect);
void backend_fill_rect(Rect *rect);
void backend_draw_text(const char *str, int x, int y, const uint8_t *font, int font_height);
void backend_present();
uint32_t backend_get_ticks();
BACKEND_EVENT backend_poll_event();
void backend_done();

#endif