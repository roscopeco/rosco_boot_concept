/*
 *------------------------------------------------------------
 *                                  ___ ___ _
 *  ___ ___ ___ ___ ___       _____|  _| . | |_
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_|
 *                     |_____|          firmware
 * ------------------------------------------------------------
 * Copyright (c)2024 The rosco_m68k Open Source Project
 * See top-level LICENSE.md for licence information.
 *
 * Xosera backend for concept boot screen
 * ------------------------------------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <machine.h>
#include <stdarg.h>

#include "xosera_m68k_api.h"

#include "view.h"
#include "backend.h"
#include "blit_rects.h"

#define BUFFER_SIZE     ((VIEW_VRES * VIEW_HRES))
#define BUFFER_LONGS    ((BUFFER_SIZE / 4))

#define BLIT_COLOR(c)   (((c << 12) | (c << 8) | (c << 4) | c))

#define XO_COLOR_BLACK              0xF000
#define XO_COLOR_WHITE              0xFFFF
#define XO_COLOR_YELLOW             0xFDD0
#define XO_COLOR_BACKGROUND         0xF234
#define XO_COLOR_BACKGROUND_SHADOW  0xF123
#define XO_COLOR_WINDOW_BACKGROUND  0xFCCC
#define XO_COLOR_SELECTION_BAR      0xF18A
#define XO_COLOR_ITEM_TEXT          0xF111
#define XO_COLOR_ITEM_HILITE_TEXT   0xFEEE

#define XO_PAGE_0_ADDR              0x0000
#define XO_PAGE_1_ADDR              0x4B00

#define XO_MAIN_FONT_ADDR           0x9600
#define XO_SMALL_FONT_ADDR          0xd600

volatile bool xosera_flip = false;
volatile uint32_t xosera_current_page = XO_PAGE_0_ADDR;

static uint8_t current_color;
static volatile uint32_t *tick_cnt = (uint32_t*)0x408;

static void dputc(char c) {
#ifndef __INTELLISENSE__
    __asm__ __volatile__(
        "move.w %[chr],%%d0\n"
        "move.l #2,%%d1\n"        // SENDCHAR
        "trap   #14\n"
        :
        : [chr] "d"(c)
        : "d0", "d1");
#endif
}

static void dprint(const char * str) {
    register char c;
    while ((c = *str++) != '\0')
    {
        if (c == '\n')
        {
            dputc('\r');
        }
        dputc(c);
    }
}

static char dprint_buff[4096];
static void dprintf(const char * fmt, ...) __attribute__((__format__(__printf__, 1, 2)));
static void dprintf(const char * fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsnprintf(dprint_buff, sizeof(dprint_buff), fmt, args);
    dprint(dprint_buff);
    va_end(args);
}

static uint32_t expand_8_pixel_font_line(uint8_t line) {
    uint32_t result = 0;

    if (line & 0x80) {
        result |= 0xF0000000;
    }

    if (line & 0x40) {
        result |= 0x0F000000;
    }

    if (line & 0x20) {
        result |= 0x00F00000;
    }

    if (line & 0x10) {
        result |= 0x000F0000;
    }

    if (line & 0x08) {
        result |= 0x0000F000;
    }

    if (line & 0x04) {
        result |= 0x00000F00;
    }

    if (line & 0x02) {
        result |= 0x000000F0;
    }

    if (line & 0x01) {
        result |= 0x0000000F;
    }

    return result;
}

bool backend_init() {
    xv_prep();

    dprintf("Calling xosera_init(XINIT_CONFIG_640x480)...");
    bool success = xosera_init(XINIT_CONFIG_640x480);
    dprintf("%s (%dx%d)\n\n", success ? "succeeded" : "FAILED", xosera_vid_width(), xosera_vid_height());

    if (!success) {
        dprintf("Exiting without Xosera init.\n");
        exit(1);
    }

    xreg_setw(PA_GFX_CTRL, 0x0055);     /* 320x240 4bpp */
    xreg_setw(PA_DISP_ADDR, 0x0000);
    xreg_setw(PA_LINE_LEN, VIEW_HRES / 4);
    xm_setw(WR_INCR, 0x0001);
    
    xmem_setw_next_addr(XR_COLOR_A_ADDR);

    // Set up palette
    xmem_setw_next(XO_COLOR_BLACK);
    xmem_setw_next(XO_COLOR_WHITE);
    xmem_setw_next(XO_COLOR_YELLOW);
    xmem_setw_next(XO_COLOR_BACKGROUND);
    xmem_setw_next(XO_COLOR_BACKGROUND_SHADOW);
    xmem_setw_next(XO_COLOR_WINDOW_BACKGROUND);
    xmem_setw_next(XO_COLOR_SELECTION_BAR);
    xmem_setw_next(XO_COLOR_ITEM_TEXT);
    xmem_setw_next(XO_COLOR_ITEM_HILITE_TEXT);

    // copy font
    xm_setw(WR_ADDR, XO_MAIN_FONT_ADDR);
    for (int i = 0; i < FONT_HEIGHT * 256; i++) {
        uint32_t expanded_line = expand_8_pixel_font_line(FONT[i]);

        xm_setw(DATA, (uint16_t)((expanded_line & 0xFFFF0000) >> 16));
        xm_setw(DATA, (uint16_t)(expanded_line & 0x0000FFFF));
    }

    // TODO copy small num font to XO_NUM_FONT_ADDR

    xosera_current_page = XO_PAGE_0_ADDR;
    xosera_flip = false;

    mcDelaymsec10(200);

    return true;
}

void backend_clear() {
    xv_prep();

    uint16_t color = BLIT_COLOR(current_color);

#ifdef OPTIMISTIC_BLITTER
    xwait_blit_ready();
#endif

    xreg_setw(BLIT_CTRL,  0x0001);                      // no transp, s-const
    xreg_setw(BLIT_ANDC,  0x0000);                      // and-complement (0xffff)
    xreg_setw(BLIT_XOR,   0x0000);                      // xor with 0x0000
    xreg_setw(BLIT_MOD_S, 0x0000);                      // constant - irrelevant
    xreg_setw(BLIT_SRC_S, color);                       // fill with current color
    xreg_setw(BLIT_MOD_D, 0x0000);                      // No skip after each line - clear full screen
    xreg_setw(BLIT_DST_D, xosera_current_page);         // Clear current page
    xreg_setw(BLIT_SHIFT, 0xFF00);                      // No blit shift
    xreg_setw(BLIT_LINES, VIEW_VRES - 1);               // All the lines
    xreg_setw(BLIT_WORDS, (VIEW_HRES / 4) - 1);         // All the pixels (at 4bpp)

#ifndef OPTIMISTIC_BLITTER
    xwait_blit_done();
#endif
}

void backend_set_color(BACKEND_COLOR color) {
    current_color = color;
}

void backend_draw_pixel(__attribute__((unused)) int x, __attribute__((unused)) int y) {
    // Currently unused...
}

void backend_text_write(const char *str, int x, int y, __attribute__((unused)) const uint8_t *font, int font_width, int font_height) {
    // TODO support different font (for small text)
    unsigned char c;

    xv_prep();

    uint16_t line_width_words = VIEW_HRES / 4;
    uint16_t width_words = font_width / 4;

    uint16_t line_mod = (VIEW_HRES / 4) - width_words;

    uint16_t color_comp = ~(BLIT_COLOR(current_color));

    printf("color: 0x%02x; comp: 0x%04x\n", current_color, color_comp);
    while ((c = *str++)) {
        const uint16_t font_ptr = XO_MAIN_FONT_ADDR + (c * font_height * 2);
        uint16_t start_word = y * line_width_words + x / 4;

#ifdef OPTIMISTIC_BLITTER
        xwait_blit_ready();
#endif

        xreg_setw(BLIT_CTRL,  0x0010);                          // 0 transp, source mem
        xreg_setw(BLIT_ANDC,  color_comp);                      // and-complement (0xffff)
        xreg_setw(BLIT_XOR,   0x0000);                          // xor with 0x0000
        xreg_setw(BLIT_MOD_S, 0x0000);                          // source data is contiguous
        xreg_setw(BLIT_SRC_S, font_ptr);                        // fill with current color
        xreg_setw(BLIT_MOD_D, line_mod);                        // No skip after each line - clear full screen
        xreg_setw(BLIT_DST_D, xosera_current_page + start_word);// Start at correct place for text
        xreg_setw(BLIT_SHIFT, 0xFF00);                          // No blit shift
        xreg_setw(BLIT_LINES, font_height - 1);                 // Whole font height (8 or 16)
        xreg_setw(BLIT_WORDS, width_words - 1);                 // Full font width (2 words)

#ifndef OPTIMISTIC_BLITTER
        xwait_blit_done();
#endif

        x += font_width;
    }
}

BACKEND_EVENT backend_poll_event() {
    if (mcCheckchar()) {
        switch (mcReadchar()) {
        case 'w':
        case 'W':
            return UP;
        case 's':
        case 'S':
            return DOWN;
        case 0x1b:
            return QUIT;
        }
    }
    return NONE;
}

uint32_t backend_get_ticks() {
    return *tick_cnt;
}

void backend_draw_rect(__attribute__((unused)) Rect *rect) {
    // Not yet
}

static inline void blit_fill_rect(
    uint16_t start_word, 
    uint16_t width_words, 
    uint16_t height_lines, 
    uint16_t line_mod
) {
    uint16_t color = BLIT_COLOR(current_color);

#ifdef OPTIMISTIC_BLITTER
    xwait_blit_ready();
#endif

    xreg_setw(BLIT_CTRL,  0x0001);                              // no transp, s-const
    xreg_setw(BLIT_ANDC,  0x0000);                              // mask nothing, and-complement (0xffff)
    xreg_setw(BLIT_XOR,   0x0000);                              // xor with 0x0000
    xreg_setw(BLIT_MOD_S, 0x0000);                              // constant - irrelevant
    xreg_setw(BLIT_SRC_S, color);                               // fill with current color
    xreg_setw(BLIT_MOD_D, line_mod);                            // skip to next line based on rect size
    xreg_setw(BLIT_DST_D, xosera_current_page + start_word);    // Start at first word of rect
    xreg_setw(BLIT_SHIFT, 0xFF00);                              // No blit shift
    xreg_setw(BLIT_LINES, height_lines - 1);                    // Whole rect
    xreg_setw(BLIT_WORDS, width_words - 1);                     // All pixels in width

#ifndef OPTIMISTIC_BLITTER
    xwait_blit_done();
#endif
}

void backend_fill_rect(Rect *rect) {
    xv_prep();

    uint16_t line_width_words = VIEW_HRES >> 2;

    BlitRects rect_lines;
    blit_lines(rect->x, rect->w, &rect_lines);

    if (rect_lines.left.width > 0) {
        uint16_t start_word = y * line_width_words + (rect->x >> 2);
        uint16_t line_mod = line_width_words - rect_lines.left.width;

        blit_fill_rect(start_word, rect_lines.left.width, rect->h, line_mod);
    }
}

void backend_present() {
    xv_prep();

    while (xosera_flip) {
        // busywait, we're going too fast (unlikely)
        //
        // This isn't used yet anyhow...
    }

    xreg_setw(PA_DISP_ADDR, xosera_current_page);

    if (xosera_current_page == XO_PAGE_0_ADDR) {
        xosera_current_page = XO_PAGE_1_ADDR;
    } else {
        xosera_current_page = XO_PAGE_0_ADDR;
    }

    // TODO write ISR to do the flip in vblank...
}

void backend_done() {
    // This space intentionally left blank
}
