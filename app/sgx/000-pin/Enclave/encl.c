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
int ecall_get_secret(int *secret_pt)
{
    int tries_left = 3, pin_attempt = 0;
    sgx_status_t ocall_rv;
    ocall_print("hello world from ecall_get_secret!");

    while (tries_left--)
    {
        ocall_rv = ocall_get_pin(&pin_attempt);
        if ((ocall_rv == SGX_SUCCESS) && (pin_attempt == super_secret_pin))
        {
            *secret_pt = super_secret_constant;
            return 1;
        }
    }

    /* Destroy secrets on 3 failed successive attempts */
    super_secret_pin      = 0x0;
    super_secret_constant = 0x0;
    return 0;
}
/* ============================ END SOLUTION ============================ */
