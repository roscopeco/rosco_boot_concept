#ifndef __TEXT_H
#define __TEXT_H

#include <stdint.h>

void text_write(const char *str, int x, int y, const uint8_t *font, int font_height);

#endif