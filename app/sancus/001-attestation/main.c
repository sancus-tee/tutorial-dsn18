#include <sancus/sm_support.h>
#include <sancus_support/sm_io.h>
#include "vulcan/drivers/mcp2515.h"
#include "foo.h"

#if SANCUS_TAG_SIZE != (CAN_PAYLOAD_SIZE*2)
    #error expecting 128-bit Sancus core
#endif

#define CAN_ATTEST_ID       0x40
#define CAN_SEND_DELAY      999999999

DECLARE_ICAN(msp_ican, /*spi_ss=*/1, CAN_500_KHZ);

int main()
{
    volatile long i;
    uint16_t can_id = 0x0;
    int can_msg_len = 0x0;
    uint8_t mac[SANCUS_TAG_SIZE] = {0x0};
    uint8_t msg[CAN_PAYLOAD_SIZE] = {0x0};

    /* ---------------------------------------------------------------------- */
    msp430_io_init();
    pr_info("initializing hardware...");
    ican_init(&msp_ican);

    pr_info("enabling foo SM...");
    sancus_enable(&foo);
    pr_sm_info(&foo);
    pr_info1("calc_foo(1) is %d\n", calc_foo(1) );

    /* ---------------------------------------------------------------------- */
    pr_info("waiting for attestation challenge CAN message...");
    can_msg_len = ican_recv(&msp_ican, &can_id, msg, /*block=*/1);
    dump_buf(msg, can_msg_len, "MSG");
    ASSERT( (can_id == CAN_ATTEST_ID) && "unexpected CAN attestation challenge message format" );

    /* ---------------------------------------------------------------------- */
    pr_info("calling into foo SM for MAC over attestation nonce...");
    /* =========================== START SOLUTION =========================== */
    /* ============================ END SOLUTION ============================ */
    dump_buf(mac, SANCUS_TAG_SIZE, "MAC");

    /* ---------------------------------------------------------------------- */
    pr_info("replying with MAC(nonce) attestation CAN message...");
    can_msg_len = 
    ican_send(&msp_ican, CAN_ATTEST_ID, &mac[0], CAN_PAYLOAD_SIZE, /*block=*/1);
    for (i=0; i < CAN_SEND_DELAY; i++);
    for (i=0; i < CAN_SEND_DELAY; i++);
    ican_send(&msp_ican, CAN_ATTEST_ID, &mac[SANCUS_TAG_SIZE/2], CAN_PAYLOAD_SIZE, /*block=*/1);

    pr_info("all done!");
    EXIT();
}
