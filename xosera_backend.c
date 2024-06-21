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

#define BUFFER_SIZE     ((VIEW_VRES * VIEW_HRES))
#define BUFFER_LONGS    ((BUFFER_SIZE / 4))

volatile bool xosera_flip;
volatile uint32_t xosera_current_page;

static uint8_t current_color;
static uint8_t buffer[BUFFER_SIZE];
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
    xmem_setw_next(0xF000);     // COLOR_BLACK
    xmem_setw_next(0xFFFF);     // COLOR_BLACK
    xmem_setw_next(0xFDD0);     // COLOR_YELLOW
    xmem_setw_next(0xF234);     // COLOR_BACKGROUND
    xmem_setw_next(0xF123);     // COLOR_BACKGROUND_SHADOW
    xmem_setw_next(0xFCCC);     // COLOR_WINDOW_BACKGROUND
    xmem_setw_next(0xF18A);     // COLOR_SELECTION_BAR
    xmem_setw_next(0xF111);     // COLOR_ITEM_TEXT
    xmem_setw_next(0xFEEE);     // COLOR_ITEM_HIGHLIGHT_TEXT

    xosera_current_page = 0;
    xosera_flip = false;

    return true;
}

void backend_clear() {
    memset(buffer, current_color, BUFFER_SIZE);
}

void backend_set_color(BACKEND_COLOR color) {
    current_color = color;
}

void backend_draw_pixel(int x, int y) {
    buffer[y * VIEW_HRES + x] = current_color;
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

void backend_draw_rect(Rect *rect) {
    int xend = rect->x + rect->w;
    int yend = rect->y + rect->h;

    // horizontal
    for (int x = rect->x; x < xend; x++) {
        backend_draw_pixel(x, rect->y);
        backend_draw_pixel(x, yend);
    }

    // vertical
    for (int y = rect->y; y < yend; y++) {
        backend_draw_pixel(rect->x, y);
        backend_draw_pixel(xend, y);
    }
}

void backend_fill_rect(Rect *rect) {
    int xend = rect->x + rect->w;
    int yend = rect->y + rect->h;

    for (int y = rect->y; y < yend; y++) {
        for (int x = rect->x; x < xend; x++) {
            backend_draw_pixel(x, y);
        }
    }
}

void backend_present() {
    xv_prep();

    while (xosera_flip) {
        // busywait, we're going too fast (unlikely)
    }

    xm_setw(WR_ADDR, xosera_current_page);

    uint32_t *buffer32 = (uint32_t*)buffer;

    for (int i = 0; i < BUFFER_LONGS; i++) {
        uint32_t pixels = buffer32[i];

        uint16_t word = ((pixels & 0x0F000000) >> 12) | ((pixels & 0x000F0000) >> 8) | ((pixels & 0x00000F00) >> 4) | (pixels & 0x0000000F);
    
        // printf("0x%08x => 0x%04x\n", pixels, word);
        xm_setw(DATA, word);
    }

    // TODO write ISR to do the flip
}

void backend_done() {
}
