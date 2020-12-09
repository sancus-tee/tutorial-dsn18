# Attestation: enclaved remote verifier application

## Application overview

After completing the `../001-attestation-unprotected` exercise, remote
verifiers can establish trust in a Sancus enclave deployed by untrusted code
over a hostile network. For this guarantee to hold, however, one should still
implicitly trust _all_ of the code running on the remote verifier's machine.
The purpose of this application is to minimize the trusted computing base
for Sancus's remote attestation protocol by encapsulating the security-sensitive
verification code in an SGX enclave.

Importantly, from the perspective of the Sancus enclave, nothing changes: you
should still be able to run the exact same `../../sancus/0001-attestation`
code, whether talking to an unprotected or enclaved remote verifier.

![attestation-overview](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-attestation.png)

**Note.** You should always strive to minimize the amount of application logic
you encapsulate in an enclave. As a general rule of thumb, application logic
that does _not_ operate on plaintext secrets (e.g., network stack) should run
outside of the enclave in order to mimimize the trusted computing base of the
application.

## Your task

Developing an enclaved version of an existing application involves some
security requirement analysis.
For the purpose of this application, we set as an explict goals that the
secret Sancus-module specific key for the `foo.c` enclave should never leak
to the untrusted world.

Based on the `../001-attestation-unprotected` source code, we provide a
complete `main.c` untrusted runtime implementation, plus the trusted
`Enclave/encl.edl` interface definitions.  Your job is to implement the
required `Enclave/encl.c` ECALL functionality.

### Providing attestation challenges

Note that, in contrast to the `../001-attestation-unprotected` application, we
cannot simply query the end user (you) for a trustworthy attestation challenge,
since all I/O operations pass via the untrusted world. We will therefore rely
on the secure hardware-level random number generation features found in modern
Intel processors.

**Do it yourself.** Implement the `ecall_get_challenge()` ECALL that returns a
64-bit random number, and securely stores it inside the enclave for use by the
next verification ECALL.

The Intel SGX SDK provides a convenient interface to access the CPU's random
number generator via the
[`sgx_read_rand`](https://software.intel.com/en-us/sgx-sdk-dev-reference-sgx-read-rand)
C function. We repeat the documentation below for convenience:

#### Syntax

```C
sgx_status_t sgx_read_rand(unsigned char *rand, size_t length_in_bytes);
```

#### Parameters

- `rand` [out] A pointer to the buffer that receives the random number. The
pointer cannot be NULL. The rand buffer can be either within or outside the
enclave, but it is not allowed to be across the enclave boundary or wrapped
around.

- `length_in_bytes` [in] The length of the buffer (in bytes).

#### Return value

`SGX_SUCCESS` Indicates success.

### Verifying attestation responses

Verifying attestation responses comes down to (i) calculating a 128-bit MAC
over the previously provided 64-bit challenge, and thereafter (ii) comparing
the expected and received MACs. In order to complete step (i), we provide a
convenient trusted [libspongent](https://github.com/sancus-pma/libspongent)
in-enclave C library to create MACs.

**Do it yourself.** Implement the `ecall_verify_response()` ECALL that
returns whether or not remote attestation succeeded.

We repeat the documentation for the relevant `libspongent` function below:

#### Syntax

```C
int spongent_mac(void* key, void* msg, unsigned int msg_len, void* mac);
```

#### Parameters

- `key` [in] Pointer to input key buffer of `SPONGENT_KEY_SIZE` bytes.
- `msg` [in] Pointer to start of associated data input buffer.
- `msg_len` [in] Length of associated data input buffer.
- `tag` [out] Pointer to output buffer of `SPONGENT_TAG_SIZE` bytes for MAC
  over associated data.

#### Return value

Returns 0 (zero) iff the wrapping succeeded.

## Building and running

First upload the `../../sancus/001-attestation` Sancus program to the MSP430
FPGA, then run the verifier program as follows:

```bash
$ make run CAN_INTERFACE=slcanN
```

**Note.** Here, slcanN refers to the CAN network interface you are using to
communicate with the MSP430 FPGA. slcan0 is the default if you ommit this
parameter.

**Note.** Since this exercise changes the source code of the `foo.c` enclave,
in order to successfully complete the remote attestation protocol in the end,
you will have to fill in the correct Sancus module-specific key in the SGX
enclave again. We refer to `../../sancus/001-attestation` for details
instructions on how to do this.
