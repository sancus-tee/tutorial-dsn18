#include "foo.h"
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

const SM_DATA(foo) int foo_magic_const = 12;

int SM_ENTRY(foo) calc_foo(int i)
{
    return i + foo_magic_const;
}

/* =========================== START SOLUTION =========================== */
int SM_ENTRY(foo) attest_foo(uint8_t *challenge, int len, uint8_t *mac)
{
    if ( !sancus_is_outside_sm(foo, challenge, len) ||
         !sancus_is_outside_sm(foo, mac, SANCUS_TAG_SIZE) )
    {
        return -1;
    }
    
    return sancus_tag(challenge, len, mac);
}
/* ============================ END SOLUTION ============================ */
