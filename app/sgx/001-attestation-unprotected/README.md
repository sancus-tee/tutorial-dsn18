# Attestation: unprotected remote verifier application

## Application overview

The purpose of this application is to allow you to get started quickly with
implementing the attestation response code on the Sancus MSP430 side.  The
program in this directory therefore completely takes care of the remote
verifier part of the remote attestation protocol (depicted below).
Specifically, the program performs the following steps:

1. Query the verifier (you) for a unique 64-bit attestation challenge *n*.
2. Construct a CAN message in the expected format destined for the MSP430
   untrusted runtime.
3. Wait for the two CAN messages with the 128-bit attestation response.
4. Compute the expected attestation response *MAC(K_N_SP_SM, n)*.
5. Compare the received and expected responses and print whether remote
   attestation succeeded or not.

![attestation-overview](https://distrinet.cs.kuleuven.be/software/sancus/images/dsn18-attestation.png)

**Note.** To get started, we focus on implementing the Sancus MSP430 side
first, and for now we completely trust the PC side. The program in this
directory therefore runs completely unprotected.
In the following exercises (`../001-attesation-enclave`), we will relax this
TCB requirement by implementing the remote stakeholder's critical attestation
verification code in an SGX enclave.

## Your task

Simply running the program suffices to send and receive CAN messages and get
started on implementing the Sancus MSP430 side of the attestation protocol.
After this is done, however, in order to successfully complete the remote
attestation protocol, you will still need to generate and fill in the Sancus
module-specific key in `main.c`. We refer to
`../../sancus/001-attestation/README.md` for detailed instructions on how to
do this.

## Building and running

First upload the `../../sancus/001-attestation` Sancus program to the MSP430
FPGA, then run the verifier program as follows:

```bash
$ make run
```
