#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include "vulcan/drivers/mcp2515.h"
#include "pmodled.h"
#include "pmodbtn.h"

#define BLINKING_DELAY  999999999
#define CAN_ID          0x40
#define MSG_LEN         CAN_PAYLOAD_SIZE

DECLARE_ICAN(msp_ican, /*spi_ss=*/1, CAN_500_KHZ);

int main()
{
    volatile long i;
    uint8_t led = 0;
    uint8_t msg[MSG_LEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};
    pmodbtn_t btn = PMOD_BTN_NONE;

    /* ---------------------------------------------------------------------- */
    msp430_io_init();
    pr_info("initializing hardware...");
    pmodled_init();
    pmodbtn_init();
    ican_init(&msp_ican);

    /* ---------------------------------------------------------------------- */
    pr_info("entering blinking led loop (press any button)..");

    while ((btn = pmodbtn_poll()) == PMOD_BTN_NONE)
    {
        pmodled_toggle(PMOD_LED1 + led);
        for (i=0; i < BLINKING_DELAY; i++);
        for (i=0; i < BLINKING_DELAY; i++);
        pmodled_toggle(PMOD_LED1 + led);

        led++;
        led = (led % 3);
    }

    /* ---------------------------------------------------------------------- */
    pr_info("press btn number to toggle led..");

    while (1) {
        pmodled_toggle(PMOD_LED0 + PMOD_BTN_TO_NUM(btn));

        if ((btn = pmodbtn_poll()) != PMOD_BTN_NONE)
        {
            pr_info("waiting for button release...");
            while (pmodbtn_poll() != PMOD_BTN_NONE);
            pr_info("button released!");

            if (btn == PMOD_BTN0)
            {
                pr_info("sending dummy CAN message (block wait)...");
                ican_send(&msp_ican, CAN_ID, msg, MSG_LEN, /*block=*/1);
                can_dump_regs(&msp_ican);
            }
        }
    }

    pr_info("all done!");
    EXIT();
}
