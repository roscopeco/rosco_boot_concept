/*
 * vim: set et ts=4 sw=4
 *------------------------------------------------------------
 *                                  ___ ___ _
 *  ___ ___ ___ ___ ___       _____|  _| . | |_
 * |  _| . |_ -|  _| . |     |     | . | . | '_|
 * |_| |___|___|___|___|_____|_|_|_|___|___|_,_|
 *                     |_____|
 * ------------------------------------------------------------
 * Copyright (c) 2021 Ross Bamford
 * MIT License
 *
 * Simple PCX loader demo for Xosera
 * ------------------------------------------------------------
 */

#include <stdlib.h>
#include <machine.h>
#include <limits.h>

#include "xosera_m68k_api.h"
#include "interlace.h"
#include "pcx.h"
#include "dprint.h"

#include "splash.h"

#define COPPER_LACE

#define ENTER_GLITCH_CHANCE     99990
#define EXIT_GLITCH_CHANCE      92500
#define GLITCH_SWITCH_CHANCE    70000

//volatile xreg_t * const xosera_ptr = (volatile xreg_t * const)0xf80060;
volatile uint32_t *tick_int = (uint32_t*)0x408;
volatile uint32_t *upticks  = (uint32_t*)0x40c;

#ifdef COPPER_LACE
void glitch_delay(uint32_t secs) {
    xv_prep();

    uint32_t end_ticks;
    if (secs == 0) {
        end_ticks = UINT_MAX;
    } else {
        end_ticks = *upticks + (secs * 100);
    }

    bool cop_on = true;
    bool in_glitch = false;

    while (*upticks < end_ticks) {
        if (in_glitch) {
            // do we want to exit the glitch?
            if ((rand() / (RAND_MAX / (100000 + 1) + 1)) > 92500) {
                // exit glitch
                cop_on = false;
                in_glitch = false;
                xwait_vblank();
                xreg_setw(COPP_CTRL, 0x0000);
            } else {
                // random glitchiness
                if ((rand() / (RAND_MAX / (100000 + 1) + 1)) > 70000) {
                    if (cop_on) {
                        cop_on = false;
                        xwait_vblank();
                        xreg_setw(COPP_CTRL, 0x0000);
                    } else {
                        cop_on = true;
                        xwait_vblank();
                        xreg_setw(COPP_CTRL, 0x8000);
                    }

                    mcDelaymsec10(2);
                }
            }
        } else {
            // do we want to enter a glitch?
            if ((rand() / (RAND_MAX / (100000 + 1) + 1)) > 99990) {
                in_glitch = true;
            }
        }
    }
}
#endif

void kmain() {
    srand(*tick_int);

    dprintf("Image is at 0x%08x (%d bytes)\n", splash_data, splash_data_len);

    xv_prep();

    dprintf("\nxosera_init(0)...");
    bool success = xosera_init(XINIT_CONFIG_848x480);
    dprintf("%s\n", success ? "succeeded" : "FAILED" /*, xv_reg_getw(vidwidth), xv_reg_getw(vidheight)*/);

    xreg_setw(PA_GFX_CTRL, 0x0065);     /* 320x240 8bpp */
    xreg_setw(PA_DISP_ADDR, 0x0000);
    xreg_setw(PA_LINE_LEN, 424 >> 1);
    xm_setw(WR_INCR, 0x0001);

    xmem_setw_next_addr(XR_COLOR_B_ADDR);
    for (int i = 0; i < 256; i++) {
        xmem_setw_next(0);
    }

    mcDelaymsec10(300);

    show_pcx(splash_data_len, splash_data, 5);

#   ifdef COPPER_LACE
    // load copper program
    xwait_vblank();
    xmem_setw_next_addr(interlace_start);
    for (uint8_t i = 0; i < interlace_size; i++)
    {
        xmem_setw_next(interlace_bin[i]);
        mcDelaymsec10(1);
    }

    xwait_vblank();
    xreg_setw(COPP_CTRL, 0x8000);
    dprintf("Copper initialized\n");

    glitch_delay(0);        
    xreg_setw(COPP_CTRL, 0x0000);
    dprintf("Done glitching\n");
#   endif

    while (true) {
        // nothing
    }
}

