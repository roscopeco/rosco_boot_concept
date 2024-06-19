#include <stdint.h>

#include "backend.h"

void text_write(const char *str, int x, int y, const uint8_t *font, int font_height) {
    unsigned char c;

    while ((c = *str++)) {
        const uint8_t *font_ptr = font + (c * font_height);

        for (int dy = 0; dy < font_height; dy++) {
            for (int dx = 0; dx < 8; dx++) {
                if ((*font_ptr & (1 << (8-dx))) != 0) {
                    backend_draw_pixel(x+dx, y+dy);
                }
            
            }

            font_ptr++;
        }

        x += 8;
    }
}
