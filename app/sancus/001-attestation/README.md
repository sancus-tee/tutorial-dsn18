# Attestation: challenge-response protocol to prove untampered protected execution

## Application overview

The purpose of this application is to cryptographically prove to a remote
stakeholder that a Sancus enclave has been loaded untampered with protection
enabled on a genuine Sancus-enabled MSP430 CPU.

![attestation-overview](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-attestation.png)

**Note.** For now, we focus exclusively on implementing the required
functionality at the Sancus MSP430 side, and we assume the remote stakeholder's
PC development side is completely trusted. In the following exercises, we will
relax this TCB requirement by leveraging off-the-shelf Intel SGX isolation
at the PC side to run the remote stakeholder's critical attestation verification
code in SGX enclaves.

## Your task

Your job is to implement remote attestation for the 'foo.c' Sancus enclave.
This requires multiple steps, outlined below.

### Implementing attestation entry point for foo enclave

In contrast to the Intel SGX SDK, the secure Sancus compiler does not require
EDL definitions, but instead requires the enclave developer to annotate C
source code with intuitive `SM_ENTRY(enclave_name)`, `SM_FUNC(enclave_name)`,
`SM_DATA(enclave_name)` macros. Only code and data explicitly marked with these
macros will be compiled into the enclave; all other code remains explicitly
untrusted.

**Do it yourself.** Add an `SM_ENTRY` enclave entry point in `foo.h` and
`foo.c`:

```C
int SM_ENTRY(foo) attest_foo(uint8_t *challenge, int len, uint8_t *mac);
```

This function takes an _untrusted_ pointer to a `challenge` buffer with length
`len`, and returns 0 iff the _untrusted_ `mac` buffer contains the 128-bit MAC
over the challenge computed with the Sancus module-specific key *K_N_SP_SM* of
the 'foo' enclave.

Note that to conveniently access the Sancus secure crypto hardware
instructions, the Sancus compiler provides a small trusted in-enclave
[`sm_support`](https://github.com/sancus-pma/sancus-compiler/blob/master/src/sancus_support/sm_support.h)
library. We repeat the documentation for the relevant function below:

#### Syntax

```C
int sancus_tag(const void* body, size_t body_len, void* tag);
```

#### Parameters

- `body` [in] Pointer to start of associated data input buffer.
- `body_len` [in] Length of associated data input buffer.
- `tag` [out] Pointer to output buffer of `SPONGENT_TAG_SIZE` bytes for MAC
  over associated data.

#### Return value

Returns true iff the wrapping succeeded.

### Extending untrusted runtime

We already provide a complete `main.c` untrusted runtime to enable protection
for the 'foo' enclave, and to properly forward 64-bit attestation challenges
and 128-bit responses over the local CAN network.  After implementing the 'foo'
remote attestation entry point, however, it should still be called from the
untrusted runtime. From the point of view of the programmer, Sancus ECALLs
resemble ordinary function calls.

**Do it yourself.** Simply insert the required `attest_foo()` function call
after receiving the attestation challenge, and before sending the response in
`main.c`.

## Building and running

As with the `000-blinking-led` example, the application's debug output can be
monitored over the tty USB serial line via the `screen` utility. At the PC
side, we provide an elementary test application to provide attestation
challenges and verify responses over the CAN bus.

### Sancus side (foo enclave and untrusted runtime)

Again, use `make load` to build and upload the application binary to the FPGA.

```bash
$ FPGA_DEV=/dev/ttyUSB0 make load # in case of permission errors, replace `USB0` with the
                                  # uplink UART USB device that was assigned to your FPGA
```

### PC side (remote stakeholder application)

We provide a minimalistic test application that can be ran at the PC side to
construct attestation challenge CAN messages and verify responses. Run as
follows:

```bash
$ cd ../../sgx/001-attestation-unprotected/
$ make run

--------------------------------------------------------------------------------
[main.c] remote attestation challenge
--------------------------------------------------------------------------------

enter attesation challenge (64-bit number) > 1234
[../common/can.c] send: CAN frame with ID 0x40 (len=8)
MSG    = d2 04 00 00 00 00 00 00 

--------------------------------------------------------------------------------
[main.c] remote attestation response
--------------------------------------------------------------------------------

[main.c] waiting for CAN response messages...
[../common/can.c] recv: CAN frame with ID 0x40 (len=8)
MSG    = df 4f 9e 35 15 9d c1 37 
[../common/can.c] recv: CAN frame with ID 0x40 (len=8)
MSG    = 73 b9 05 9f 1e 28 25 22 
SM_MAC = df 4f 9e 35 15 9d c1 37 73 b9 05 9f 1e 28 25 22 

--------------------------------------------------------------------------------
[main.c] comparing expected and received MACs...
--------------------------------------------------------------------------------

[main.c] calculating expected MAC...
MY_MAC = df 4f 9e 35 15 9d c1 37 73 b9 05 9f 1e 28 25 22 
[main.c] OK   : remote attestation succeeded!

```

### Providing attestation challenges

The above application simply queries you for a 64-bit number. This number is
subsequently included as the payload for an attestation challenge CAN frame
destined for the remote Sancus enclave. Only a properly loaded foo enclave
will have access to the correct Sancus module-specific key that is required
to construct the 128-bit response MAC over the challenge.

**Note.** Be aware that attackers with full control over the network can easily
record and replay plaintext challenges and their corresponding MAC responses. To
thwart such replay attacks, you will have to establish a _fresh_ attestation
guarantee every time you run the application (i.e., the challenge you enter
serves as a nonce and in real-world deployment, you should make sure to never
re-use the same attestation challenge).

### Verifying attestation responses

If all goes well and you implemented the required functionality at the Sancus
side, the above program should still terminate with a failed attestation
response verification. This is expected behavior, for we should still provide
the PC application with the correct 128-bit Sancus module-specific key.

We rely on the `sancus-crypto` utility to compute the Sancus module-specific
key for the 'foo' module, deployed with software provider identity `0x1234` on
the FPGA with 128-bit node master key `0xdeadbeefcafebabec0defeeddefec8ed`.

```bash
# First, we derive the vendor key from the node master key
$ sancus-crypto --key deadbeefcafebabec0defeeddefec8ed --gen-vendor-key 1234
0b7bf3ae40880a8be430d0da34fb76f0

# Now, we derive the module-specific key for the foo module
$ sancus-crypto --key 0b7bf3ae40880a8be430d0da34fb76f0 --gen-sm-key foo --c-array main.elf
uint8_t key[] = { 0x05, 0x41, 0x87, 0x10, 0xff, 0xa4, 0x83, 0xc1, 0x2e, 0xe2, 0xda, 0x82, 0x81, 0x36, 0xb7, 0x6b};
```

To successfully complete the remote attestation process, fill in this key in 
`../../sgx/001-attestation/main.c`, and run the Sancus and PC applications again.

**Note.** The above Sancus module-specific key includes foo's compiled code
section, such that you will have to repeat the above process each time when you
change foo's source code. Any changes in foo's code will lead to a different
key and will thus be detected by the remote stakeholder at the PC side; this is
exactly what attestation is all about.
