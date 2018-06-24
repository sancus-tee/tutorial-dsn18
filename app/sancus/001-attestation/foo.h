#ifndef SM_FOO_H_INC
#define SM_FOO_H_INC

#include <sancus/sm_support.h>
#include <stdint.h>

extern struct SancusModule foo;

int SM_ENTRY(foo) calc_foo(int i);

/* =========================== START SOLUTION =========================== */
int SM_ENTRY(foo) attest_foo(uint8_t *challenge, int len, uint8_t *mac);
/* ============================ END SOLUTION ============================ */

#endif
