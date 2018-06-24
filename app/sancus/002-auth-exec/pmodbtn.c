#include "pmodbtn.h"
#include <msp430.h>

DECLARE_EXCLUSIVE_MMIO_SM(pmodbtn,
                          /*[secret_start, end[=*/ P3IN_, P4IN_,
                          /*caller_id=*/ 1,
                          /*vendor_id=*/ 0x1234);

uint8_t SM_MMIO_ENTRY(pmodbtn) pmodbtn_poll(void)
{
    asm(
    "mov.b &%0, r15                                                     \n\t" 
    ::"m"(P3IN)
    );
}

void SM_MMIO_ENTRY(pmodbtn) pmodbtn_init(void)
{
    /*
     * Our setup: BTN0123 mapped to P3IN[4:7]
     *
     * https://store.digilentinc.com/pmodbtn-4-user-pushbuttons/
     * http://processors.wiki.ti.com/index.php/Digital_I/O_(MSP430)
     */
    asm(
    "mov.b #0x00, &%0                                                   \n\t" 
    "mov.b #0x00, &%1                                                   \n\t"
    ::"m"(P3SEL), "m"(P3DIR):
    );
}
