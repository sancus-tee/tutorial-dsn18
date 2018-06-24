#include "foo.h"
#include <sancus_support/sm_io.h>

DECLARE_SM(foo, 0x1234);

const SM_DATA(foo) int foo_magic_const = 12;

int SM_ENTRY(foo) calc_foo(int i)
{
    return i + foo_magic_const;
}

/* =========================== START SOLUTION =========================== */

/* ============================ END SOLUTION ============================ */
