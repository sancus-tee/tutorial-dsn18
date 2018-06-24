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
    /* ============================ END SOLUTION ============================ */
}
