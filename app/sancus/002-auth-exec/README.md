# Authentic execution: secure remote sensor-actuator distributed enclave application

## Application overview

Low-end embedded microcontrollers are commonly interfaced with peripheral I/O
devices via memory-mapped I/O. Since Sancus features a generic memory isolation
primitive, small dedicated Sancus driver assembly enclaves can be provided with
exclusive access to peripheral I/O devices. In this exercise, we will leverage
Sancus's local attestation primitives to extend the remote attestation protocol
of the 'foo' enclave with evidence of an authentic PmodBTN driver sensor
reading (and PmodLED driver actuation).

## Your task

We provide you with a fully implemented `main.c` untrusted runtime to take care
of the CAN network communication, plus two trusted `pmodbtn.c` and `pmodled.c`
Sancus driver enclaves to securely sense/actuate the PmodBTN/LED peripherals.

**Do it yourself.** Based on your `../001-attestation` implementation, extend
the `attest_foo` entry point to compute a MAC over both the provided
attestation challenge, plus a local sensor reading from the 'pmodbtn' Sancus
driver enclave. The signature of the extended foo entry point looks as follows:

```C
int SM_ENTRY(foo) attest_foo(uint8_t *challenge, int len, uint16_t *btn, uint8_t *mac)
```

This function takes an _untrusted_ pointer to a `challenge` buffer with length
`len`, and returns 0 iff the _untrusted_ `mac` buffer contains the 128-bit MAC
and the _untrusted_ `btn` buffer contains the 16-bit PmodBTN output.
Particularly, your implementation should perform the following steps:

1. If called for the first time, initialize the drive enclaves by calling the
   `pmodbtn_init()` and `pmodled_init()` entry points.
2. Fetch a PmodBTN sensor reading by calling the `pmodbtn_poll()` entry point
   of the 'PmodBTN' driver enclave.
3. (optional) Actuate the corresponding LED by calling the `pmodled_actuate`
   entry point of the 'PmodLED' driver enclave.
4. Convert the 8-bit sensor reading into a 16-bit driver state (for padding
   when calculating the MAC later) and store it in the untrusted `btn` output
   buffer.
5. Initialize an internal 10-byte `msg` buffer with the concatenation of the
   64-bit attestation challenge, and the 16-bit driver state.
6. Compute the MAC over `msg` and store it in the untrusted `mac` output
   buffer.

**Note.** The Sancus compiler transparently takes care of local attestation
whenever encountering cross-enclave function calls. Merely calling the driver
enclaves from the foo enclave thus suffices to establish their integrity.

### Authentic sensor readings via Sancus PmodBTN driver enclave

We provide you with a fully working PmodBTN driver enclave that
offers two entry points:

1. `pmodbtn_init`: should be called to initialize the driver enclave before
                   requesting sensor readings.
2. `pmodbtn_poll`: returns an 8-bit bitmap indicating which buttons were down.

### Authentic actuation via Sancus PmodLED driver enclave

We provide you with a fully working PmodLED driver enclave that
offers two entry points:

1. `pmodled_init`:    should be called to initialize the driver enclave before
                      issuing sensor actuation calls.
2. `pmodled_actuate`: lights up the LEDs according to the 8-bit bitmap argument.

## Building and running

### Sancus side (foo/PmodBTN/PmodLED enclaves and untrusted runtime)

Again, use `make load` to build and upload the application binary to the FPGA.

```bash
$ FPGA_DEV=/dev/ttyUSB0 make load # in case of permission errors, replace `USB0` with the
                                  # uplink UART USB device that was assigned to your FPGA
```

### PC side (remote stakeholder application)

```bash
$ cd ../../sgx/002-auth-exec/
$ make run
```

**Note.** Since this exercise changes the source code of the `foo.c` enclave,
in order to successfully complete the remote attestation protocol in the end,
you will have to fill in the correct Sancus module-specific key in the SGX
enclave again. We refer to `../001-attestation` for details instructions on how
to do this.
