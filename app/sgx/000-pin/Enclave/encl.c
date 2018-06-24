#include "encl_t.h"

/*
 * NOTE: for demonstration purposes, we hard-code secrets at compile time and
 * abstract away how they are securely provisioned at runtime.
 */
int super_secret_constant   = 0xdeadbeef;
int super_secret_pin        = 1234;

int ecall_dummy(int i)
{
    ocall_print("hello world from ecall_dummy!");
    return super_secret_constant + i;
}

/* =========================== START SOLUTION =========================== */

/* ============================ END SOLUTION ============================ */
