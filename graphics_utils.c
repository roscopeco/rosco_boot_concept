#include <stdlib.h>

#include "graphics_utils.h"

bool gfx_measure_text(char *text, int *out_width_chars, int *out_height_lines) {
    if (text == NULL) {
        return false;
    }

    int line_count = 1;
    int current_len = 0;
    int longest_len = 0;

    while (true) {
        char c = *text++;

        if (c == 0) {
            if (current_len > longest_len) {
                longest_len = current_len;
            }

            break;
        } else if (c == '\n') {
            line_count++;

            if (current_len > longest_len) {
                longest_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
    }

    *out_width_chars = longest_len;

    // If input was zero length, measure it as 0,0
    if (longest_len == 0 && line_count == 1) {
        *out_height_lines = 0;
    } else {
        *out_height_lines = line_count;
    }

    return true;
}