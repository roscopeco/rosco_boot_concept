#include <stdint.h>

#include <rosco_m68k/xosera_m68k_api.h>
#include <rosco_m68k/machine.h>

#include "dprintf.h"
#include "bong.h"

#define BUFFER_A            XR_TILE_ADDR
#define BUFFER_B            ((XR_TILE_ADDR+BUFFER_SIZE))
#define BUFFER_SIZE         0x400

static volatile xmreg_t * xosera_ptr;

int main() {
    const int rate = 11025;

    while (mcCheckchar())
    {
        mcReadchar();
    }

    xosera_ptr = ((volatile xmreg_t *)(((*((volatile uint32_t*)SDB_XOSERABASE)))));

    dprintf("Calling xosera_init(XINIT_CONFIG_848x480)...");
    bool success = xosera_init(XINIT_CONFIG_848x480);

    if (!success) {
        dprintf("Xosera init failed!\n");
        return 100;
    }

    uint32_t clk_hz = xosera_sample_hz();
    uint16_t period = (clk_hz + rate - 1) / rate;        // rate is samples per second
    dprintf("Period is %d\n", period);

    xreg_setw(AUD_CTRL, AUD_CTRL_AUD_EN_F);
    xreg_setw(AUD0_PERIOD, period);
    xreg_setw(AUD0_VOL, MAKE_AUD_VOL(AUD_VOL_FULL / 2, AUD_VOL_FULL / 2));

    bool prime = true;
    bool done = false;
    uint32_t src_ptr = 0;
    uint32_t current_buf = BUFFER_A;

    while (true) {
        if (prime || (xm_getw(INT_CTRL) & INT_CTRL_AUD0_INTR_F)) {
            xm_setw(INT_CTRL, INT_CTRL_AUD_ALL_F);
            prime = false;

            if (done) {
                break;
            }

            // copy data to buffer
            int copy_len = BUFFER_SIZE;
            int data_remain = bong_data_len - src_ptr;
            if (data_remain < BUFFER_SIZE) {
                dprintf("Fetched last chunk of %d\n", data_remain);
                copy_len = data_remain;
                done = true;        // Signal done next time we get an interrupt pending...
            }
            copy_len /= 2;

            xm_setw(WR_XADDR, current_buf);

            for (int i = 0; i < copy_len; i++) {
                uint16_t word = bong_data[src_ptr++] << 8;
                word |= bong_data[src_ptr++];

                xm_setw(XDATA, word);
            }            

            xreg_setw(AUD0_LENGTH, (copy_len - 1) | AUD_LENGTH_TILEMEM_F);
            xreg_setw(AUD0_START, current_buf);

            current_buf = (current_buf == BUFFER_A) ? BUFFER_B : BUFFER_A;
        }
    }

    xreg_setw(AUD_CTRL, 0x0000);
    dprintf("All done");
}
