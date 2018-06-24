# SGX enclave applications

This directory collects the tutorial code to be ran on an SGX-capable Intel x86 CPU.

## Exercises overview

Exercises are organized in increasing complexity as follows:

- **000-pin:** basic example enclave program demonstrating ECALL/OCALL concepts.
- **001-attestation-unprotected:** basic _unprotected_ remote verifier
  application to quickly start implementing Sancus MSP430 side of the remote
  attestation protocol.
- **001-attestation:** enclaved version of the remote verifier application.
- **002-auth-exec:** secure remote sensor-actuator distributed enclave application.

## 000: PIN code verifier

The purpose of this application is to develop a first elementary SGX enclave
program that compares a secret in-enclave PIN code against user input obtained
from the untrusted world.

**Your task.** Extend the trusted `Enclave/encl.edl` public interface,
implement the required ECALL `Enclave/encl.c` functionality, and extend the
untrusted `main.c` runtime to call into the enclave.

**Goals.** After this exercise, you should be able to:

* Build and run elementary SGX x86 applications.
* Understand ECALL and OCALL concepts to securely transfer control flow
  to/from enclave mode.
* Understand limitations of SGX's enclaved execution and the need for secure I/O.

## 001: Remote attestation (unprotected)

The purpose of this application is to provide you with a fully working
_unprotected_ implementation of the remote verifier part of the remote
attestation protocol. This allows you on the one hand to get started quickly
with implementing the attestation response code (`../sancus/001-attestation`)
on the Sancus MSP430 side. On the other hand, in the next exercise, we will
use the unprotected code as a starting point to develop a hardened enclaved
version of the remote verifier's implementation.

**Your task.** Simply run the program to send and receive CAN messages, and get
started on implementing the Sancus MSP430 side of the attestation protocol.
Thereafter, in order to successfully complete the remote attestation protocol,
you will still need to generate and fill in the Sancus module-specific key in
`main.c`.

**Goals.** After this exercise, you should be able to:

* Understand remote attestation concepts.
* Thwart basic protocol-level replay attacks via the proper use of nonces.
* Understand trusted computing base concerns and why it is undesirable to
  trust the entire remote verifier's PC software stack.

## 001: Remote attestation (enclaved)

The purpose of this application is to minimize the trusted computing base for
Sancus's remote attestation protocol by encapsulating the security-sensitive
remote verifier's code in an SGX enclave, such that the secret Sancus
module-specific `foo.c` attestation key never leaks to the untrusted world.

**Your task.** Modify the `001-attestation-unprotected` application to identify
and protect the security-relevant application parts.

**Goals.** After this exercise, you should be able to:

* Identify and encapsulate security-sensitive application logic in an SGX
  enclave, and reason about the resulting trusted computing base.
* Securely generate random numbers in an SGX enclave.
* Understand the dangers of untrusted pointer dereferences in enclave programs.
* Understand "transitive trust" limitations of attesting the Sancus MSP430
  side within an SGX enclave (i.e., who guarantees the untampered execution of
  the SGX enclave in its turn?).

## 002: Authentic execution

The purpose of this application is to show how remote attestation can be
leveraged in practice to authenticate the results of remote computations
(sensor readings) in a hostile environment.

**Your task.** Modify the `001-attestation` application to verify an extended
MAC over both the attestation challenge and a remote sensor reading.

**Goals.** After this exercise, you should be able to:

* Understand authentic execution concepts.
* Understand how trust in a remote platform and its sensor readings can be
  established by combining remote attestation of an application enclave
  with local attestation of driver enclaves residing on the same device.
