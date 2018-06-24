#include "pmodled.h"
#include <msp430.h>

DECLARE_EXCLUSIVE_MMIO_SM(pmodled,
                          /*[secret_start, end[=*/ P1IN_, P2IN_,
                          /*caller_id=*/ 1,
                          /*vendor_id=*/ 0x1234);

void SM_MMIO_ENTRY(pmodled) pmodled_actuate(uint8_t state)
{
    asm(
    "mov.b r15, &%0                                                      \n\t" 
    ::"m"(P1OUT)
    );
}

void SM_MMIO_ENTRY(pmodled) pmodled_init(void)
{
    /*
     * Our setup: LD0 permanently grounded, LD123 mapped to P1OUT[5:7]
     *
     * https://store.digilentinc.com/pmod-led-four-high-brightness-leds/
     * http://processors.wiki.ti.com/index.php/Digital_I/O_(MSP430)
     */
    asm(
    "mov.b #0x00, &%0                                                   \n\t" 
    "mov.b #0xff, &%1                                                   \n\t"
    ::"m"(P1SEL), "m"(P1DIR):
    );
}
