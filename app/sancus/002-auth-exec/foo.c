#include "foo.h"
#include "pmodbtn.h"
#include "pmodled.h"
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

const SM_DATA(foo) int foo_magic_const = 12;

int SM_ENTRY(foo) calc_foo(int i)
{
    return i + foo_magic_const;
}

/*
 * We create a MAC over: challenge (8B) | PMODBTN sensor reading (2B)
 *
 * NOTE: we use a 2B sensor reading size to ensure the total size of the buffer
 * is a multiple of 2 (to work around a known erratum in the Sancus crypto HW
 * instructions).
 */
#define ATTEST_MSG_SIZE (sizeof(uint64_t) + sizeof(uint16_t))

uint8_t SM_DATA(foo) msg[ATTEST_MSG_SIZE] = {0x0};
int     SM_DATA(foo) foo_drv_init = 0;

int SM_ENTRY(foo) attest_foo(uint8_t *challenge, int len, uint16_t *btn, uint8_t *mac)
{
    /* =========================== START SOLUTION =========================== */
    int i;
    uint16_t driver_state = 0x00;
    if ( !sancus_is_outside_sm(foo, challenge, len) ||
         !sancus_is_outside_sm(foo, mac, SANCUS_TAG_SIZE) ||
         !sancus_is_outside_sm(foo, btn, sizeof(uint16_t)) ||
         (len > (ATTEST_MSG_SIZE-1)) )
    {
        pr_info("invalid parameters!");
        return -1;
    }

    /* 0. initialize secure PMODBTN driver */
    if (!foo_drv_init)
    {
        pr_info("initializing PMODBTN/PMODLED driver enclaves..");
        pmodbtn_init();
        pmodled_init();
        foo_drv_init = 1;
    }

    /* 1. copy untrusted CAN payload into attestation buffer */
    for (i=0; i < ATTEST_MSG_SIZE; i++)
        msg[i] = (i < len) ? challenge[i] : 0x0;
    
    /*
     * 2. locally attest PMODBTN driver (implicit via compiler-generated
     * ecall), request sensor reading, and copy into attestation buffer
     */
    pr_info("fetching PMODBTN driver state..");
    driver_state = pmodbtn_poll();
    pr_info1("driver says 0x%x\n", driver_state);
    *btn = msg[ATTEST_MSG_SIZE-1] = driver_state;

    /*
     * 3. locally attest PMODLED driver (implicit via compiler-generated
     * ecall), and securely actuate corresponding LED.
     */
    pmodled_actuate(driver_state);
    
    /* 4. authenticate attestation buffer */
    pr_info("computing attestation MAC..");
    dump_buf(msg, ATTEST_MSG_SIZE, "ATTEST_MSG");
    return sancus_tag(msg, ATTEST_MSG_SIZE, mac);
    /* ============================ END SOLUTION ============================ */
}
