#ifndef PMOD_LED_H_INC
#define PMOD_LED_H_INC

#include <msp430.h>

/* Our setup: LD0 permanently grounded, LD123 mapped to P1OUT[5:7] */
typedef enum {
    PMOD_LED0 = 4,
    PMOD_LED1 = 5,
    PMOD_LED2 = 6,
    PMOD_LED3 = 7,
} pmodled_t;

#define PMOD_LED_TO_NUM( led )  (led - PMOD_LED0)
#define PMOD_LED_TO_MASK( led ) (0x1 << led)

void pmodled_init(void);
void pmodled_toggle(pmodled_t led);

#endif
