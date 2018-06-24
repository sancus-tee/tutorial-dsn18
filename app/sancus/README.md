# Sancus enclave applications

This directory collects the tutorial code to be ran on
[Sancus](https://distrinet.cs.kuleuven.be/software/sancus/)-enabled MSP430
MCUs.

## Exercises overview

Exercises are organized in increasing complexity as follows:

- **000-blinking-led:** basic _unprotected_ application to test tutorial hardware setup.
- **001-attestation:**  cryptographically prove untampered protected execution
                         to remote stakeholders.
- **002-auth-exec:**    TODO

## 000: Blinking LEDs

The classic hardware-level "hello world" counterpart. The purpose of this
application is to test the basic hardware setup for an _unprotected_
example program (i.e., without leveraging the secure Sancus CPU extensions yet). 

**Your task.** Build and run the unmodified example program, play around with
the buttons, and inspect associated debug and CAN output.

**Goals.** After this exercise, you should be able to:

* Build and upload C applications to the MSP430 FPGA.
* Inspect debug output from the FPGA via the serial `/dev/ttyUSB1` UART
  connection.
* Inspect CAN messages from the FPGA with the `candump` utility.

## 001: Remote attestation

The purpose of this application is to cryptographically prove to a remote
stakeholder that her Sancus enclave has been loaded untampered with protection
enabled on a genuine Sancus-enabled MSP430 CPU.

**Your task.** Implement remote attestation for the `foo.c` Sancus enclave, and
extend the untrusted `main.c` runtime to properly forward attestation
challenges and responses over the local CAN network.

**Goals.** After this exercise, you should be able to:

* Understand remote attestation concepts.
* Create minimalistic Sancus enclaves with ecalls (`SM_ENTRY`), private
  data (`SM_DATA`), and function calls (`SM_FUNC`).
* Thwart basic protocol-level replay attacks via the proper use of nonces.
* Understand the dangers of untrusted pointer dereferences in enclave programs.

## 002: Authentic execution

The purpose of this application is to extend the remote attestation protocol of
the previous exercise to include proof of an authentic local PmodBTN sensor
reading (plus corresponding PmodLED actuation event).

**Goals.** After this exercise, you should be able to:

* Understand local attestation concepts and transitive trust relations.
* Securely link to Sancus secure I/O driver enclaves.
