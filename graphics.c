#include <string.h>

#include "graphics.h"
#include "view.h"

#define STR_BUFFER_SIZE     256

static char buffer[STR_BUFFER_SIZE];

void gfx_render_text(char *text, int x, int y, BACKEND_FONT_COOKIE font, int font_width, int font_height) {
    if (text == NULL || strncpy(buffer, text, STR_BUFFER_SIZE - 1) == NULL) {
        return;
    }

    char *line = strtok(buffer, "\n");
    int current_y = y;
    
    while (line) {
        backend_text_write(line, x, current_y, font, font_width, font_height);
        current_y += LINE_HEIGHT;
        line = strtok(NULL, "\n");
    }
}
