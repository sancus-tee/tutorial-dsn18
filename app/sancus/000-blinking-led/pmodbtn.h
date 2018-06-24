#ifndef PMOD_BTN_H_INC
#define PMOD_BTN_H_INC

#include <msp430.h>

/* Our setup: BTN0123 mapped to P3IN[4:7] */
typedef enum {
    PMOD_BTN_NONE   = 0,
    PMOD_BTN3       = 4,
    PMOD_BTN2       = 5,
    PMOD_BTN1       = 6,
    PMOD_BTN0       = 7,
} pmodbtn_t;

#define PMOD_BTN_TO_NUM( btn ) (PMOD_BTN0 - btn)

void pmodbtn_init(void);
pmodbtn_t pmodbtn_poll(void);

#endif
