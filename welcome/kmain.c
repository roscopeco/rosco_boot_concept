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

#include "xosera_m68k_api.h"
#include "interlace.h"
#include "pcx.h"
#include "dprint.h"

#include "splash.h"

#define COPPER_LACE

//volatile xreg_t * const xosera_ptr = (volatile xreg_t * const)0xf80060;

extern void *_image_start;
extern void *_image_end;

void kmain() {
    dprintf("Image is at 0x%08x (%d bytes)\n", splash_data, splash_data_len);

    xv_prep();

    dprintf("\nxosera_init(0)...");
    bool success = xosera_init(XINIT_CONFIG_848x480);
    dprintf("%s\n", success ? "succeeded" : "FAILED" /*, xv_reg_getw(vidwidth), xv_reg_getw(vidheight)*/);

    xreg_setw(PA_GFX_CTRL, 0x0065);     /* 320x240 8bpp */
    xreg_setw(PA_DISP_ADDR, 0x0000);
    xreg_setw(PA_LINE_LEN, 424 >> 1);
    xm_setw(WR_INCR, 0x0001);

    show_pcx(splash_data_len, splash_data);

#   ifdef COPPER_LACE
    // load copper program
    xmem_setw_next_addr(interlace_start);
    for (uint8_t i = 0; i < interlace_size; i++)
    {
        xmem_setw_next(interlace_bin[i]);
    }

    xreg_setw(COPP_CTRL, 0x8000);    
    dprintf("Copper initialized\n");
#   endif

    while (true) {}
}

