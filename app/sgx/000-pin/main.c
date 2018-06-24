/* utility headers */
#include "debug.h"

/* SGX untrusted runtime */
#include <sgx_urts.h>
#include "Enclave/encl_u.h"

/* define untrusted OCALL functions here */

void ocall_print(const char *str)
{
    info("ocall_print: enclave says: '%s'", str);
}

int ocall_get_pin(void)
{
    int pin = 0;
    printf("enter PIN code > ");
    scanf("%d", &pin);    
    return pin;
}

sgx_enclave_id_t create_enclave(void)
{
    sgx_launch_token_t token = {0};
    int updated = 0;
    sgx_enclave_id_t eid = -1;

    info_event("Creating enclave...");
    SGX_ASSERT( sgx_create_enclave( "./Enclave/encl.so", /*debug=*/1,
                                    &token, &updated, &eid, NULL ) );

    return eid;
}

int main( int argc, char **argv )
{
    sgx_enclave_id_t eid = create_enclave();
    int rv = 1, secret = 0;
    
    /* ---------------------------------------------------------------------- */
    info_event("calling enclave...");

    SGX_ASSERT( ecall_dummy(eid, &rv, 1) );
    info("ecall_dummy(1) enclave entry point returned %d", rv);

    /* =========================== START SOLUTION =========================== */
    SGX_ASSERT( ecall_get_secret(eid, &rv, &secret) );
    info("ecall_get_secret enclave entry point returned %d (secret 0x%x)",
        rv, secret);
    /* ============================ END SOLUTION ============================ */

    /* ---------------------------------------------------------------------- */
    info_event("destroying SGX enclave");
    SGX_ASSERT( sgx_destroy_enclave( eid ) );

    info("all is well; exiting..");
	return 0;
}
