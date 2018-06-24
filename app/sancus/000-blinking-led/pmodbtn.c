#include "pmodbtn.h"
#include <stdio.h>

int pmodbtn_initialized = 0;

void pmodbtn_init(void)
{
    /*
     * Our setup: BTN0123 mapped to P3IN[4:7]
     *
     * https://store.digilentinc.com/pmodbtn-4-user-pushbuttons/
     * http://processors.wiki.ti.com/index.php/Digital_I/O_(MSP430)
     */
    P3SEL = 0x00;
    P3DIR = 0x00;
    P3OUT = 0x00;

    pmodbtn_initialized = 1;
}

pmodbtn_t pmodbtn_poll(void)
{
    int i, state;
    pmodbtn_t rv = PMOD_BTN_NONE;

    if (!pmodbtn_initialized)
        pmodbtn_init();

    state = P3IN;

    /*
     * Return the highest numerical button which is pressed.
     *
     * NOTE: no early-out on first hit to avoid timing side-channels
     */
    for (i = PMOD_BTN0; i >= PMOD_BTN3; i--)
    {
        if (state & (0x1 << i))
        {
            printf("[pmodnbtn] BTN %d pressed!\n", PMOD_BTN_TO_NUM(i) );
            rv = i;
        }
    }

    return rv;
}
