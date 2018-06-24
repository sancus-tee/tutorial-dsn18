#include "pmodled.h"
#include <stdio.h>

int pmodled_initialized = 0;

void pmodled_init(void)
{
    /*
     * Our setup: LD0 permanently grounded, LD123 mapped to P1OUT[5:7]
     *
     * https://store.digilentinc.com/pmod-led-four-high-brightness-leds/
     * http://processors.wiki.ti.com/index.php/Digital_I/O_(MSP430)
     */
    P1SEL = 0x00;
    P1DIR = 0xff;
    P1OUT = 0x00;

    pmodled_initialized = 1;
}

void pmodled_toggle(pmodled_t led)
{
    if (led < PMOD_LED1 || led > PMOD_LED3)
        return;

    printf("[pmodled] toggling LED %d \n", PMOD_LED_TO_NUM(led));

    if (!pmodled_initialized)
        pmodled_init();

    int toggled = P1OUT & PMOD_LED_TO_MASK(led);

    if (!toggled)
        P1OUT |= PMOD_LED_TO_MASK(led);
    else
        P1OUT &= ~PMOD_LED_TO_MASK(led);
}
