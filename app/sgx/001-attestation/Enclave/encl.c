#include "libspongent/spongewrap.h"
#include <sgx_trts.h>
#include "encl_t.h"

/*
 * XXX Fill in the correct Sancus-specific key computed from the final foo.c enclave source code:
 *
 * $ sancus-crypto --key 0b7bf3ae40880a8be430d0da34fb76f0 --gen-sm-key foo --c-array main.elf
 */
uint8_t key[] = { 0x39, 0x8f, 0x2b, 0x56, 0x31, 0x0d, 0x58, 0xaa, 0x8c, 0xf5, 0x09, 0x23, 0x89, 0x31, 0xcb, 0x7b};

/* =========================== START SOLUTION =========================== */
uint64_t last_challenge = 0x0;
/* ============================ END SOLUTION ============================ */

uint64_t ecall_get_challenge(void)
{
    /* =========================== START SOLUTION =========================== */
    if (sgx_read_rand((uint8_t*) &last_challenge, sizeof(uint64_t)) == SGX_SUCCESS)
        return last_challenge;
    else
        return -1;
    /* ============================ END SOLUTION ============================ */
}

int ecall_verify_response(uint8_t *sm_mac)
{
    /* =========================== START SOLUTION =========================== */
    uint8_t my_mac[SPONGENT_TAG_SIZE] = {0x0};
    int i;

    if ( spongent_mac((void*) key, (uint8_t*) &last_challenge, sizeof(uint64_t), my_mac) )
        return 0;

    for (i = 0; (i < SPONGENT_TAG_SIZE) && (my_mac[i] == sm_mac[i]); i++);

    return (i >= SPONGENT_TAG_SIZE);
    /* ============================ END SOLUTION ============================ */
}   
